#include "application.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace strelnikov
{

  void UndoManager::push(std::unique_ptr< Action > action)
  {
    undoStack_.push_back(std::move(action));
    redoStack_.clear();
  }

  void UndoManager::undo(FileSystem &fs)
  {
    if (undoStack_.empty()) {
      throw std::logic_error("nothing to undo");
    }
    auto action = std::move(undoStack_.back());
    undoStack_.pop_back();
    action->undo(fs);
    redoStack_.push_back(std::move(action));
  }

  void UndoManager::redo(FileSystem &fs)
  {
    if (redoStack_.empty()) {
      throw std::logic_error("nothing to redo");
    }
    auto action = std::move(redoStack_.back());
    redoStack_.pop_back();
    action->execute(fs);
    undoStack_.push_back(std::move(action));
  }

  bool UndoManager::canUndo() const
  {
    return !undoStack_.empty();
  }

  bool UndoManager::canRedo() const
  {
    return !redoStack_.empty();
  }

  void UndoManager::clearRedo()
  {
    redoStack_.clear();
  }

  void UndoManager::clearUndo()
  {
    undoStack_.clear();
  }

  std::string UndoManager::peekUndo() const
  {
    return undoStack_.back()->getCommandName();
  }

  std::string UndoManager::peekRedo() const
  {
    return redoStack_.back()->getCommandName();
  }

  std::vector< std::string > UndoManager::getUndoStack()
  {
    std::vector< std::string > result;
    for (auto it = undoStack_.rbegin(); it != undoStack_.rend(); ++it) {
      result.push_back((*it)->getCommandName());
    }
    return result;
  }

  std::vector< std::string > UndoManager::getRedoStack()
  {
    std::vector< std::string > result;
    for (auto it = redoStack_.rbegin(); it != redoStack_.rend(); ++it) {
      result.push_back((*it)->getCommandName());
    }
    return result;
  }

  Application::Application():
    serviceCommands_{{"undo", &Application::handleUndo},
                     {"dryUndo", &Application::handleDryUndo},
                     {"print-undo", &Application::handlePrintUndoStack},
                     {"redo", &Application::handleRedo},
                     {"dryRedo", &Application::handleDryRedo},
                     {"print-redo", &Application::handlePrintRedoStack},
                     {"history", &Application::handleHistory},
                     {"save", &Application::handleSave},
                     {"load", &Application::handleLoad}}
  {
    registerAllActions();
  }

  void Application::run()
  {
    std::string line;

    while (std::getline(std::cin, line)) {
      if (line.empty())
        continue;
      Command cmd = parser_.parse(line);
      if (!cmd.isValid) {
        std::cout << "[ERR] INVALID COMMAND: " << cmd.errorMessage << "\n";
        continue;
      }
      processCommand(cmd, line);
    }
  }

  void Application::processCommand(const Command &cmd, const std::string &rawInput)
  {
    try {
      auto it = serviceCommands_.find(cmd.name);

      if (it != serviceCommands_.end()) {
        it->second(this, cmd.args.empty() ? "" : cmd.args[0]);
      } else {
        undoManager_.clearRedo();

        auto action = Action::create(cmd.name, cmd.args);
        action->execute(fs_);
        undoManager_.push(std::move(action));
        commandHistory_.push_back(rawInput);
      }
    } catch (const std::exception &e) {
      std::cout << "[ERR] INVALID COMMAND: " << e.what() << "\n";
    }
  }

  void Application::handleUndo(Application *app, const std::string &)
  {
    if (!app->undoManager_.canUndo()) {
      std::cout << "[ERR] INVALID COMMAND: nothing to undo\n";
    } else {
      app->undoManager_.undo(app->fs_);
    }
  }

  void Application::handleDryUndo(Application *app, const std::string &)
  {
    if (!app->undoManager_.canUndo()) {
      std::cout << "[OK] NOTHING TO UNDO\n";
    } else {
      std::cout << "[OK] NEXT UNDO: " << app->undoManager_.peekUndo() << "\n";
    }
  }

  void Application::handlePrintUndoStack(Application *app, const std::string &)
  {
    if (!app->undoManager_.canUndo()) {
      std::cout << "[OK] NOTHING TO UNDO\n";
    } else {
      std::vector< std::string > stack = app->undoManager_.getUndoStack();

      std::cout << "[OK] UNDO QUEUE (" << stack.size() << " actions):\n";
      for (size_t i = 0; i < stack.size(); ++i) {
        std::cout << "  " << (i + 1) << ". " << stack[i] << "\n";
      }
    }
  }

  void Application::handleDryRedo(Application *app, const std::string &)
  {
    if (!app->undoManager_.canRedo()) {
      std::cout << "[OK] NOTHING TO REDO\n";
    } else {
      std::cout << "[OK] NEXT REDO: " << app->undoManager_.peekRedo() << "\n";
    }
  }

  void Application::handlePrintRedoStack(Application *app, const std::string &)
  {
    if (!app->undoManager_.canRedo()) {
      std::cout << "[OK] NOTHING TO REDO\n";
    } else {
      std::vector< std::string > stack = app->undoManager_.getRedoStack();

      std::cout << "[OK] REDO QUEUE (" << stack.size() << " actions):\n";
      for (size_t i = 0; i < stack.size(); ++i) {
        std::cout << "  " << (i + 1) << ". " << stack[i] << "\n";
      }
    }
  }

  void Application::handleRedo(Application *app, const std::string &)
  {
    if (!app->undoManager_.canRedo()) {
      std::cout << "[ERR] INVALID COMMAND: nothing to redo\n";
    } else {
      app->undoManager_.redo(app->fs_);
    }
  }

  void Application::handleHistory(Application *app, const std::string &)
  {
    if (app->commandHistory_.empty()) {
      std::cout << "[OK] NOTHING TO SHOW\n";
    } else {
      for (const auto &cmdStr : app->commandHistory_) {
        std::cout << cmdStr << "\n";
      }
    }
  }

  void Application::handleSave(Application *app, const std::string &arg)
  {
    app->handleSaveLogic(arg);
  }

  void Application::handleLoad(Application *app, const std::string &arg)
  {
    app->undoManager_.clearRedo();
    app->undoManager_.clearUndo();

    app->handleLoadLogic(arg);
  }

  void Application::handleSaveLogic(const std::string &filename)
  {
    std::ofstream out(filename);
    if (!out)
      throw std::runtime_error("cannot open file for saving");

    for (const auto &pair : fs_.getTags()) {
      out << "make-tag " << pair.first << "\n";
    }

    for (const auto &pair : fs_.getCollections()) {
      const std::string &name = pair.first;
      const auto &coll = pair.second;
      out << "make-collection " << name << "\n";
      for (const auto &t : coll->getTags()) {
        out << "give-tag " << t << " " << name << "\n";
      }
    }

    for (const auto &pair : fs_.getFiles()) {
      const std::string &path = pair.first;
      const auto &file = pair.second;
      std::string ownerColl = "";
      for (const auto &cPair : fs_.getCollections()) {
        const std::string &cName = cPair.first;
        const auto &coll = cPair.second;
        if (coll->hasFile(path)) {
          ownerColl = cName;
          break;
        }
      }
      if (!ownerColl.empty()) {
        out << "add-file " << ownerColl << " " << path << "\n";

        for (const auto &t : file->getTags()) {
          out << "give-tag " << t << " " << path << "\n";
        }
      }
    }
  }

  void Application::handleLoadLogic(const std::string &filename)
  {
    std::ifstream in(filename);
    if (!in)
      throw std::runtime_error("cannot open file for loading");

    std::string line;
    while (std::getline(in, line)) {
      if (line.empty())
        continue;
      Command cmd = parser_.parse(line);
      if (cmd.isValid) {
        try {
          if (cmd.name == "save" || cmd.name == "load" || cmd.name == "history" || cmd.name == "undo"
              || cmd.name == "redo") {
            continue;
          }
          auto action = Action::create(cmd.name, cmd.args);
          action->execute(fs_);

        } catch (const std::exception &e) {

          std::cout << "[WRN] LOAD FAILED: " << e.what() << " (command: " << line << ")\n";
        }
      }
    }
  }
}
