#include "actions.hpp"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <memory>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>
#include "structs.hpp"

namespace strelnikov
{
  std::unordered_map< std::string, Action::Creator > &Action::getRegistry()
  {
    static std::unordered_map< std::string, Creator > registry;
    return registry;
  }

  std::unique_ptr< Action > Action::create(const std::string &name, const std::vector< std::string > &args)
  {
    auto it = getRegistry().find(name);
    if (it == getRegistry().end()) {
      throw std::invalid_argument("unknown command: " + name);
    }
    return it->second(args);
  }

  std::string getFilename(const std::string &path)
  {
    size_t pos = path.find_last_of("/\\");
    if (pos == std::string::npos)
      return path;
    return path.substr(pos + 1);
  }

  static bool startsWith(const std::string &str, const std::string &prefix)
  {
    return str.size() >= prefix.size() && str.compare(0, prefix.size(), prefix) == 0;
  }

  MakeCollectionAction::MakeCollectionAction(const std::vector< std::string > &args)
  {
    if (args.size() != 1)
      throw std::invalid_argument("invalid arguments");
    name_ = args[0];
  }
  void MakeCollectionAction::execute(FileSystem &fs)
  {
    fs.createCollection(name_);
    std::cout << "[OK] COLLECTION \"" << name_ << "\" CREATED\n";
  }
  void MakeCollectionAction::undo(FileSystem &fs)
  {
    fs.removeCollection(name_);
    std::cout << "[OK] COLLECTION \"" << name_ << "\" DELETED\n";
  }
  std::string MakeCollectionAction::getCommandName() const
  {
    return "make-collection " + name_;
  }

  RmCollectionAction::RmCollectionAction(const std::vector< std::string > &args)
  {
    if (args.size() != 1)
      throw std::invalid_argument("invalid arguments");
    name_ = args[0];
  }
  void RmCollectionAction::execute(FileSystem &fs)
  {
    fs.removeCollection(name_);
    std::cout << "[OK] COLLECTION \"" << name_ << "\" DELETED\n";
  }
  void RmCollectionAction::undo(FileSystem &fs)
  {
    fs.createCollection(name_);
    std::cout << "[OK] COLLECTION \"" << name_ << "\" CREATED\n";
  }
  std::string RmCollectionAction::getCommandName() const
  {
    return "rm-collection " + name_;
  }

  ListCollectionsAction::ListCollectionsAction(const std::vector< std::string > &)
  {}
  void ListCollectionsAction::execute(FileSystem &fs)
  {
    std::cout << "[OK] AVAILABLE COLLECTIONS:\n";
    auto colls = fs.getCollections();
    if (colls.empty()) {
      std::cout << "[OK] NOT FOUND\n";
      return;
    }
    for (const auto &pair : colls) {
      const std::string &name = pair.first;
      const auto &coll = pair.second;
      std::cout << "[OK] " << name << " FILES=" << coll->getFiles().size() << " TAGS=\"";
      const auto &tags = coll->getTags();
      for (size_t i = 0; i < tags.size(); ++i)
        std::cout << tags[i] << (i + 1 < tags.size() ? " " : "");
      std::cout << "\"\n";
    }
  }
  void ListCollectionsAction::undo(FileSystem &)
  {}
  std::string ListCollectionsAction::getCommandName() const
  {
    return "list-collections";
  }

  AddFileAction::AddFileAction(const std::vector< std::string > &args)
  {
    if (args.size() != 2)
      throw std::invalid_argument("invalid arguments");
    collectionName_ = args[0];
    filePath_ = normalizePath(args[1]);
  }
  void AddFileAction::execute(FileSystem &fs)
  {
    try {
      fs.createFile(filePath_);
    } catch (...) {
    }
    fs.addFileToCollection(collectionName_, filePath_);
    std::cout << "[OK] ADDED \"" << filePath_ << "\" into \"" << collectionName_ << "\" COLLECTION\n";
  }
  void AddFileAction::undo(FileSystem &fs)
  {
    fs.removeFileFromCollection(collectionName_, filePath_);
    std::cout << "[OK] DELETED \"" << filePath_ << "\" from \"" << collectionName_ << "\" COLLECTION\n";
  }
  std::string AddFileAction::getCommandName() const
  {
    return "add-file " + collectionName_ + " " + filePath_;
  }

  RmFileAction::RmFileAction(const std::vector< std::string > &args)
  {
    if (args.size() != 2)
      throw std::invalid_argument("invalid arguments");
    collectionName_ = args[0];
    identifier_ = args[1];
  }
  void RmFileAction::execute(FileSystem &fs)
  {
    auto it = fs.getCollections().find(collectionName_);
    if (it == fs.getCollections().end())
      throw std::invalid_argument("collection \"" + collectionName_ + "\" does not exist");
    auto coll = it->second;
    std::string normId = normalizePath(identifier_);
    if (coll->hasFile(normId)) {
      resolvedPath_ = normId;
    } else {
      std::vector< std::string > matches;
      for (const auto &f : coll->getFiles()) {
        if (getFilename(f) == identifier_)
          matches.push_back(f);
      }
      if (matches.size() > 1) {
        std::cout << "[ERR] AMBIGUOUS IDENTIFIER\n";
        return;
      } else if (matches.size() == 1) {
        resolvedPath_ = matches[0];
      }
    }
    if (resolvedPath_.empty()) {
      std::cout << "[WRN] NOTHING HAPPENED: file \"" << identifier_ << "\" does not exist in \"" << collectionName_
                << "\" collection\n";
      return;
    }
    fs.removeFileFromCollection(collectionName_, resolvedPath_);
    std::cout << "[OK] DELETED \"" << resolvedPath_ << "\" from \"" << collectionName_ << "\" COLLECTION\n";
  }
  void RmFileAction::undo(FileSystem &fs)
  {
    if (!resolvedPath_.empty()) {
      try {
        fs.createFile(resolvedPath_);
      } catch (...) {
      }
      fs.addFileToCollection(collectionName_, resolvedPath_);
      std::cout << "[OK] ADDED \"" << resolvedPath_ << "\" into \"" << collectionName_ << "\" COLLECTION\n";
    }
  }
  std::string RmFileAction::getCommandName() const
  {
    return "rm-file " + collectionName_ + " " + identifier_;
  }

  ShowAction::ShowAction(const std::vector< std::string > &args)
  {
    if (args.size() != 1)
      throw std::invalid_argument("invalid arguments");
    collectionName_ = args[0];
  }
  void ShowAction::execute(FileSystem &fs)
  {
    auto it = fs.getCollections().find(collectionName_);
    if (it == fs.getCollections().end())
      throw std::invalid_argument("collection \"" + collectionName_ + "\" does not exist");
    auto coll = it->second;
    if (coll->getFiles().empty()) {
      std::cout << "[OK] COLLECTION IS BLANK\n";
      return;
    }
    for (const auto &fPath : coll->getFiles()) {
      auto fileIt = fs.getFiles().find(fPath);
      std::string tagsStr = "";
      if (fileIt != fs.getFiles().end()) {
        const auto &tags = fileIt->second->getTags();
        for (size_t i = 0; i < tags.size(); ++i)
          tagsStr += tags[i] + (i + 1 < tags.size() ? " " : "");
      }
      std::string fname = getFilename(fPath);
      std::cout << "[OK] " << fname << " TAGS=\"" << tagsStr << "\"\n";
    }
  }
  void ShowAction::undo(FileSystem &)
  {}
  std::string ShowAction::getCommandName() const
  {
    return "show " + collectionName_;
  }

  SortAction::SortAction(const std::vector< std::string > &args)
  {
    if (args.size() != 2)
      throw std::invalid_argument("invalid arguments");
    collectionName_ = args[0];
    order_ = args[1];
  }
  void SortAction::execute(FileSystem &fs)
  {
    std::cout << "\033[31m[WRN] SORT DISABLED\033[0m\n";
    return;
    if (order_ != "by-name") {
      std::cout << "[ERR] INVALID COMMAND: order \"" << order_ << "\" is not supported. Use 'by-name'\n";
      return;
    }
    auto it = fs.getCollections().find(collectionName_);
    if (it == fs.getCollections().end())
      throw std::invalid_argument("collection \"" + collectionName_ + "\" does not exist");
    auto coll = it->second;
    auto files = coll->getFiles();
    std::sort(files.begin(), files.end());
    for (const auto &f : coll->getFiles())
      coll->removeFile(f);
    for (const auto &f : files)
      coll->addFile(f);
    std::cout << "[OK] COLLECTION \"" << collectionName_ << "\" WAS SORTED\n";
  }
  void SortAction::undo(FileSystem &)
  {}
  std::string SortAction::getCommandName() const
  {
    return "sort " + collectionName_ + " " + order_;
  }

  CheckFilesAction::CheckFilesAction(const std::vector< std::string > &args)
  {
    if (args.empty() || args.size() > 2)
      throw std::invalid_argument("invalid arguments");
    collectionName_ = args[0];
    if (args.size() == 2)
      batchAction_ = args[1];
  }
  void CheckFilesAction::execute(FileSystem &fs)
  {
    auto it = fs.getCollections().find(collectionName_);
    if (it == fs.getCollections().end())
      throw std::invalid_argument("collection \"" + collectionName_ + "\" does not exist");
    auto coll = it->second;
    auto filesToCheck = coll->getFiles();
    for (const auto &filePath : filesToCheck) {
      auto fileIt = fs.getFiles().find(filePath);
      bool accessible = (fileIt != fs.getFiles().end() && fileIt->second->validate());
      if (accessible) {
        std::cout << "[OK] FILE \"" << filePath << "\" IS ACCESSIBLE\n";
      } else {
        std::cout << "[WRN] FILE \"" << filePath << "\" IS MISSING OR INACCESSIBLE\n";
        std::string action = batchAction_;
        if (action.empty()) {
          std::cout << "[ACTION] decisions made here cannot be reversed (rm/mv/SKIP): ";
          std::string input;
          if (!std::getline(std::cin, input))
            action = "skip";
          else
            action = input;
        }
        if (action == "rm") {
          fs.removeFileFromCollection(collectionName_, filePath);
          std::cout << "[OK] RECORD FOR \"" << filePath << "\" REMOVED FROM \"" << collectionName_ << "\" COLLECTION\n";
        } else if (action.size() > 3 && action.substr(0, 3) == "mv ") {
          std::string newPath = normalizePath(action.substr(3));
          fs.renameFile(filePath, newPath);
          std::cout << "[OK] RECORD FOR \"" << filePath << "\" MOVED TO \"" << newPath << "\" IN \"" << collectionName_
                    << "\" COLLECTION\n";
        }
      }
    }
  }
  void CheckFilesAction::undo(FileSystem &)
  {}
  std::string CheckFilesAction::getCommandName() const
  {
    return "check-files " + collectionName_ + " " + batchAction_;
  }

  MakeTagAction::MakeTagAction(const std::vector< std::string > &args)
  {
    if (args.size() != 1)
      throw std::invalid_argument("invalid arguments");
    tagName_ = args[0];
  }
  void MakeTagAction::execute(FileSystem &fs)
  {
    fs.createTag(tagName_);
    std::cout << "[OK] TAG \"" << tagName_ << "\" CREATED\n";
  }
  void MakeTagAction::undo(FileSystem &fs)
  {
    fs.removeTag(tagName_);
    std::cout << "[OK] TAG \"" << tagName_ << "\" DELETED\n";
  }
  std::string MakeTagAction::getCommandName() const
  {
    return "make-tag " + tagName_;
  }

  RmTagAction::RmTagAction(const std::vector< std::string > &args)
  {
    if (args.size() != 1)
      throw std::invalid_argument("invalid arguments");
    tagName_ = args[0];
  }
  void RmTagAction::execute(FileSystem &fs)
  {
    fs.removeTag(tagName_);
    std::cout << "[OK] TAG \"" << tagName_ << "\" DELETED\n";
  }
  void RmTagAction::undo(FileSystem &fs)
  {
    fs.createTag(tagName_);
    std::cout << "[OK] TAG \"" << tagName_ << "\" CREATED\n";
  }
  std::string RmTagAction::getCommandName() const
  {
    return "rm-tag " + tagName_;
  }

  GiveTagAction::GiveTagAction(const std::vector< std::string > &args):
    tagName_(args[0]),
    targetName_(args[1])
  {}
  void GiveTagAction::execute(FileSystem &fs)
  {
    if (!fs.getTags().count(tagName_)) {
      throw std::invalid_argument("tag \"" + tagName_ + "\" does not exist");
    }

    if (fs.getCollections().count(targetName_)) {
      fs.giveTagToCollection(targetName_, tagName_);
      std::cout << "[OK] tag \"" << tagName_ << "\" was assigned to \"" << targetName_ << "\" collection" << '\n';
      return;
    }

    std::string normPath = normalizePath(targetName_);
    std::vector< std::string > matches;

    if (fs.getFiles().count(normPath)) {
      matches.push_back(normPath);
    } else {
      for (const auto &pair : fs.getFiles()) {
        if (getFilename(pair.first) == targetName_) {
          matches.push_back(pair.first);
        }
      }
    }

    if (matches.empty()) {
      throw std::invalid_argument("collection or file \"" + targetName_ + "\" does not exist");
    }
    if (matches.size() > 1) {
      std::cout << "[ERR] AMBIGUOUS IDENTIFIER" << '\n';
      return;
    }

    resolvedPath_ = matches[0];
    fs.giveTagToFile(resolvedPath_, tagName_);
    std::cout << "[OK] tag \"" << tagName_ << "\" was assigned to \"" << resolvedPath_ << "\" file" << '\n';
  }
  void GiveTagAction::undo(FileSystem &fs)
  {
    if (!resolvedPath_.empty()) {
      try {
        fs.withdrawTagFromFile(resolvedPath_, tagName_);
        std::cout << "[OK] tag \"" << tagName_ << "\" was withdrawned to \"" << targetName_ << "\" file" << '\n';
      } catch (...) {
      }
    } else {
      try {
        fs.withdrawTagFromCollection(targetName_, tagName_);
        std::cout << "[OK] tag \"" << tagName_ << "\" was withdrawned to \"" << targetName_ << "\" collection" << '\n';
      } catch (...) {
      }
    }
  }
  std::string GiveTagAction::getCommandName() const
  {
    return "give-tag " + tagName_ + " " + targetName_;
  }

  WithdrawTagAction::WithdrawTagAction(const std::vector< std::string > &args):
    tagName_(args[0]),
    targetName_(args[1])
  {}
  void WithdrawTagAction::execute(FileSystem &fs)
  {
    if (!fs.getTags().count(tagName_)) {
      throw std::invalid_argument("tag \"" + tagName_ + "\" does not exist");
    }

    if (fs.getCollections().count(targetName_)) {
      fs.withdrawTagFromCollection(targetName_, tagName_);
      std::cout << "[OK] tag \"" << tagName_ << "\" was withdrawned to \"" << targetName_ << "\" collection" << '\n';
      return;
    }

    std::string normPath = normalizePath(targetName_);
    std::vector< std::string > matches;

    if (fs.getFiles().count(normPath)) {
      matches.push_back(normPath);
    } else {
      for (const auto &pair : fs.getFiles()) {
        if (getFilename(pair.first) == targetName_) {
          matches.push_back(pair.first);
        }
      }
    }

    if (matches.empty()) {
      throw std::invalid_argument("collection or file \"" + targetName_ + "\" does not exist");
    }
    if (matches.size() > 1) {
      std::cout << "[ERR] AMBIGUOUS IDENTIFIER" << '\n';
      return;
    }

    resolvedPath_ = matches[0];
    fs.giveTagToFile(resolvedPath_, tagName_);
    std::cout << "[OK] tag \"" << tagName_ << "\" was withdrawned to \"" << resolvedPath_ << "\" file" << '\n';
  }

  void WithdrawTagAction::undo(FileSystem &fs)
  {
    if (!resolvedPath_.empty()) {
      try {
        fs.giveTagToFile(resolvedPath_, tagName_);
        std::cout << "[OK] tag \"" << tagName_ << "\" was assigned to \"" << targetName_ << "\" file" << '\n';
      } catch (...) {
      }
    } else {
      try {
        fs.giveTagToCollection(targetName_, tagName_);
        std::cout << "[OK] tag \"" << tagName_ << "\" was assigned to \"" << targetName_ << "\" collection" << '\n';
      } catch (...) {
      }
    }
  }
  std::string WithdrawTagAction::getCommandName() const
  {
    return "withdraw-tag " + tagName_ + " " + targetName_;
  }

  FindFileByNameAction::FindFileByNameAction(const std::vector< std::string > &args)
  {
    if (args.empty() || args.size() > 2)
      throw std::invalid_argument("invalid arguments");
    fullname_ = args[0];
    if (args.size() == 2)
      collectionName_ = args[1];
  }
  void FindFileByNameAction::execute(FileSystem &fs)
  {
    std::cout << "[OK] FILES WITH \"" << fullname_ << "\" NAME:\n";
    bool found = false;
    for (const auto &pair : fs.getFiles()) {
      const std::string &path = pair.first;
      if (getFilename(path) == fullname_ || path == fullname_) {
        if (collectionName_.empty() || fs.getCollections().at(collectionName_)->hasFile(path)) {
          std::cout << "[OK] " << path << "\n";
          found = true;
        }
      }
    }
    if (!found)
      std::cout << "[OK] NOT FOUND\n";
  }
  void FindFileByNameAction::undo(FileSystem &)
  {}
  std::string FindFileByNameAction::getCommandName() const
  {
    return "find-file-by-name " + collectionName_ + " " + fullname_;
  }

  FindFileByPrefixAction::FindFileByPrefixAction(const std::vector< std::string > &args)
  {
    if (args.empty() || args.size() > 2)
      throw std::invalid_argument("invalid arguments");
    prefix_ = args[0];
    if (args.size() == 2)
      collectionName_ = args[1];
  }
  void FindFileByPrefixAction::execute(FileSystem &fs)
  {
    std::cout << "[OK] FILES WITH \"" << prefix_ << "\" PREFIX:\n";
    bool found = false;
    for (const auto &pair : fs.getFiles()) {
      const std::string &path = pair.first;
      if (startsWith(getFilename(path), prefix_) || startsWith(path, prefix_)) {
        if (collectionName_.empty() || fs.getCollections().at(collectionName_)->hasFile(path)) {
          std::cout << "[OK] " << path << "\n";
          found = true;
        }
      }
    }
    if (!found)
      std::cout << "[OK] NOT FOUND\n";
  }
  void FindFileByPrefixAction::undo(FileSystem &)
  {}
  std::string FindFileByPrefixAction::getCommandName() const
  {
    return "find-file-by-prefix " + collectionName_ + " " + prefix_;
  }

  FindFileByTagAction::FindFileByTagAction(const std::vector< std::string > &args)
  {
    if (args.empty() || args.size() > 2)
      throw std::invalid_argument("invalid arguments");
    tagName_ = args[0];
    if (args.size() == 2)
      collectionName_ = args[1];
  }
  void FindFileByTagAction::execute(FileSystem &fs)
  {
    if (!fs.getTags().count(tagName_))
      throw std::invalid_argument("tag \"" + tagName_ + "\" does not exist");
    std::cout << "[OK] FILES WITH \"" << tagName_ << "\" TAG:\n";
    bool found = false;
    for (const auto &pair : fs.getFiles()) {
      const std::string &path = pair.first;
      const auto &file = pair.second;
      if (file->hasTag(tagName_)) {
        if (collectionName_.empty() || fs.getCollections().at(collectionName_)->hasFile(path)) {
          std::cout << "[OK] " << path << "\n";
          found = true;
        }
      }
    }
    if (!found)
      std::cout << "[OK] NOT FOUND\n";
  }
  void FindFileByTagAction::undo(FileSystem &)
  {}
  std::string FindFileByTagAction::getCommandName() const
  {
    return "find-file-by-tag " + collectionName_ + " " + tagName_;
  }

  FindTagByNameAction::FindTagByNameAction(const std::vector< std::string > &args)
  {
    if (args.size() != 1)
      throw std::invalid_argument("invalid arguments");
    tagName_ = args[0];
  }
  void FindTagByNameAction::execute(FileSystem &fs)
  {
    std::cout << "[OK] TAGS WITH \"" << tagName_ << "\" NAME:\n";
    if (fs.getTags().count(tagName_)) {
      int count = 0;
      for (const auto &pair : fs.getFiles()) {
        const auto &f = pair.second;
        if (f->hasTag(tagName_))
          count++;
      }
      for (const auto &pair : fs.getCollections()) {
        const auto &col = pair.second;
        if (col->hasTag(tagName_))
          count++;
      }
      std::cout << "[OK] " << tagName_ << " USED_TIMES=" << count << "\n";
    } else
      std::cout << "[OK] NOT FOUND\n";
  }
  void FindTagByNameAction::undo(FileSystem &)
  {}
  std::string FindTagByNameAction::getCommandName() const
  {
    return "find-tag-by-name " + tagName_;
  }

  FindTagByPrefixAction::FindTagByPrefixAction(const std::vector< std::string > &args)
  {
    if (args.size() != 1)
      throw std::invalid_argument("invalid arguments");
    prefix_ = args[0];
  }
  void FindTagByPrefixAction::execute(FileSystem &fs)
  {
    std::cout << "[OK] TAGS WITH \"" << prefix_ << "\" PREFIX:\n";
    bool found = false;
    for (const auto &pair : fs.getTags()) {
      const std::string &name = pair.first;
      if (startsWith(name, prefix_)) {
        int count = 0;
        for (const auto &fPair : fs.getFiles()) {
          const auto &f = fPair.second;
          if (f->hasTag(name))
            count++;
        }
        for (const auto &cPair : fs.getCollections()) {
          const auto &col = cPair.second;
          if (col->hasTag(name))
            count++;
        }
        std::cout << "[OK] " << name << " USED_TIMES=" << count << "\n";
        found = true;
      }
    }
    if (!found)
      std::cout << "[OK] NOT FOUND\n";
  }
  void FindTagByPrefixAction::undo(FileSystem &)
  {}
  std::string FindTagByPrefixAction::getCommandName() const
  {
    return "find-tag-by-prefix " + prefix_;
  }

  FindCollectionByNameAction::FindCollectionByNameAction(const std::vector< std::string > &args)
  {
    if (args.size() != 1)
      throw std::invalid_argument("invalid arguments");
    collectionName_ = args[0];
  }
  void FindCollectionByNameAction::execute(FileSystem &fs)
  {
    std::cout << "[OK] COLLECTIONS WITH \"" << collectionName_ << "\" NAME:\n";
    auto it = fs.getCollections().find(collectionName_);
    if (it != fs.getCollections().end()) {
      auto coll = it->second;
      std::cout << "[OK] " << collectionName_ << " FILES=" << coll->getFiles().size() << " TAGS=\"";
      const auto &tags = coll->getTags();
      for (size_t i = 0; i < tags.size(); ++i)
        std::cout << tags[i] << (i + 1 < tags.size() ? " " : "");
      std::cout << "\"\n";
    } else
      std::cout << "[OK] NOT FOUND\n";
  }
  void FindCollectionByNameAction::undo(FileSystem &)
  {}
  std::string FindCollectionByNameAction::getCommandName() const
  {
    return "find-collection-by-name " + collectionName_;
  }

  FindCollectionByPrefixAction::FindCollectionByPrefixAction(const std::vector< std::string > &args)
  {
    if (args.size() != 1)
      throw std::invalid_argument("invalid arguments");
    prefix_ = args[0];
  }
  void FindCollectionByPrefixAction::execute(FileSystem &fs)
  {
    std::cout << "[OK] COLLECTIONS WITH \"" << prefix_ << "\" PREFIX:\n";
    bool found = false;
    for (const auto &pair : fs.getCollections()) {
      const std::string &name = pair.first;
      const auto &coll = pair.second;
      if (startsWith(name, prefix_)) {
        std::cout << "[OK] " << name << " FILES=" << coll->getFiles().size() << " TAGS=\"";
        const auto &tags = coll->getTags();
        for (size_t i = 0; i < tags.size(); ++i)
          std::cout << tags[i] << (i + 1 < tags.size() ? " " : "");
        std::cout << "\"\n";
        found = true;
      }
    }
    if (!found)
      std::cout << "[OK] NOT FOUND\n";
  }
  void FindCollectionByPrefixAction::undo(FileSystem &)
  {}
  std::string FindCollectionByPrefixAction::getCommandName() const
  {
    return "find-collection-by-prefix " + prefix_;
  }

  UnionCollectionsAction::UnionCollectionsAction(const std::vector< std::string > &args)
  {
    if (args.size() != 3)
      throw std::invalid_argument("invalid arguments");
    newName_ = args[0];
    col1_ = args[1];
    col2_ = args[2];
  }
  void UnionCollectionsAction::execute(FileSystem &fs)
  {
    if (fs.getCollections().count(newName_))
      throw std::invalid_argument("collection with name \"" + newName_ + "\" already exists");
    if (!fs.getCollections().count(col1_))
      throw std::invalid_argument("collection \"" + col1_ + "\" does not exist");
    if (!fs.getCollections().count(col2_))
      throw std::invalid_argument("collection \"" + col2_ + "\" does not exist");
    fs.createCollection(newName_);
    auto c1 = fs.getCollections().at(col1_);
    auto c2 = fs.getCollections().at(col2_);
    for (const auto &f : c1->getFiles())
      fs.addFileToCollection(newName_, f);
    for (const auto &f : c2->getFiles()) {
      try {
        fs.addFileToCollection(newName_, f);
      } catch (...) {
      }
    }
    std::cout << "[OK] COLLECTIONS MERGED INTO \"" << newName_ << "\"\n";
  }
  void UnionCollectionsAction::undo(FileSystem &fs)
  {
    fs.removeCollection(newName_);
  }
  std::string UnionCollectionsAction::getCommandName() const
  {
    return "union-collections " + newName_ + " " + col1_ + " " + col2_;
  }

  IntersectCollectionsAction::IntersectCollectionsAction(const std::vector< std::string > &args)
  {
    if (args.size() != 3)
      throw std::invalid_argument("invalid arguments");
    newName_ = args[0];
    col1_ = args[1];
    col2_ = args[2];
  }
  void IntersectCollectionsAction::execute(FileSystem &fs)
  {
    if (fs.getCollections().count(newName_))
      throw std::invalid_argument("collection with name \"" + newName_ + "\" already exists");
    if (!fs.getCollections().count(col1_))
      throw std::invalid_argument("collection \"" + col1_ + "\" does not exist");
    if (!fs.getCollections().count(col2_))
      throw std::invalid_argument("collection \"" + col2_ + "\" does not exist");
    fs.createCollection(newName_);
    auto c1 = fs.getCollections().at(col1_);
    auto c2 = fs.getCollections().at(col2_);
    for (const auto &f : c1->getFiles())
      if (c2->hasFile(f))
        fs.addFileToCollection(newName_, f);
    std::cout << "[OK] COLLECTIONS INTERSECTED INTO \"" << newName_ << "\"\n";
  }
  void IntersectCollectionsAction::undo(FileSystem &fs)
  {
    fs.removeCollection(newName_);
  }
  std::string IntersectCollectionsAction::getCommandName() const
  {
    return "intersect-collections " + newName_ + " " + col1_ + " " + col2_;
  }

  FilterByTagAction::FilterByTagAction(const std::vector< std::string > &args)
  {
    if (args.size() != 3)
      throw std::invalid_argument("invalid arguments");
    newName_ = args[0];
    sourceCol_ = args[1];
    tagName_ = args[2];
  }
  void FilterByTagAction::execute(FileSystem &fs)
  {
    if (fs.getCollections().count(newName_))
      throw std::invalid_argument("collection with name \"" + newName_ + "\" already exists");
    if (!fs.getTags().count(tagName_))
      throw std::invalid_argument("tag \"" + tagName_ + "\" does not exist");
    if (!fs.getCollections().count(sourceCol_))
      throw std::invalid_argument("collection \"" + sourceCol_ + "\" does not exist");
    fs.createCollection(newName_);
    auto src = fs.getCollections().at(sourceCol_);
    for (const auto &f : src->getFiles()) {
      if (fs.getFiles().at(f)->hasTag(tagName_))
        fs.addFileToCollection(newName_, f);
    }
    std::cout << "[OK] COLLECTION \"" << newName_ << "\" CREATED FROM \"" << sourceCol_ << "\" WITH TAG \"" << tagName_
              << "\"\n";
  }
  void FilterByTagAction::undo(FileSystem &fs)
  {
    fs.removeCollection(newName_);
  }
  std::string FilterByTagAction::getCommandName() const
  {
    return "filter-by-tag " + newName_ + " " + sourceCol_ + " " + tagName_;
  }

  void registerAllActions()
  {
    auto &reg = Action::getRegistry();
    reg["make-collection"] = [](const std::vector< std::string > &args) -> std::unique_ptr< Action > {
      return std::unique_ptr< MakeCollectionAction >(new MakeCollectionAction(args));
    };
    reg["rm-collection"] = [](const std::vector< std::string > &args) -> std::unique_ptr< Action > {
      return std::unique_ptr< RmCollectionAction >(new RmCollectionAction(args));
    };
    reg["list-collections"] = [](const std::vector< std::string > &args) -> std::unique_ptr< Action > {
      return std::unique_ptr< ListCollectionsAction >(new ListCollectionsAction(args));
    };
    reg["add-file"] = [](const std::vector< std::string > &args) -> std::unique_ptr< Action > {
      return std::unique_ptr< AddFileAction >(new AddFileAction(args));
    };
    reg["rm-file"] = [](const std::vector< std::string > &args) -> std::unique_ptr< Action > {
      return std::unique_ptr< RmFileAction >(new RmFileAction(args));
    };
    reg["show"] = [](const std::vector< std::string > &args) -> std::unique_ptr< Action > {
      return std::unique_ptr< ShowAction >(new ShowAction(args));
    };
    reg["sort"] = [](const std::vector< std::string > &args) -> std::unique_ptr< Action > {
      return std::unique_ptr< SortAction >(new SortAction(args));
    };
    reg["check-files"] = [](const std::vector< std::string > &args) -> std::unique_ptr< Action > {
      return std::unique_ptr< CheckFilesAction >(new CheckFilesAction(args));
    };
    reg["make-tag"] = [](const std::vector< std::string > &args) -> std::unique_ptr< Action > {
      return std::unique_ptr< MakeTagAction >(new MakeTagAction(args));
    };
    reg["rm-tag"] = [](const std::vector< std::string > &args) -> std::unique_ptr< Action > {
      return std::unique_ptr< RmTagAction >(new RmTagAction(args));
    };
    reg["give-tag"] = [](const std::vector< std::string > &args) -> std::unique_ptr< Action > {
      return std::unique_ptr< GiveTagAction >(new GiveTagAction(args));
    };
    reg["withdraw-tag"] = [](const std::vector< std::string > &args) -> std::unique_ptr< Action > {
      return std::unique_ptr< WithdrawTagAction >(new WithdrawTagAction(args));
    };
    reg["find-file-by-name"] = [](const std::vector< std::string > &args) -> std::unique_ptr< Action > {
      return std::unique_ptr< FindFileByNameAction >(new FindFileByNameAction(args));
    };
    reg["find-file-by-prefix"] = [](const std::vector< std::string > &args) -> std::unique_ptr< Action > {
      return std::unique_ptr< FindFileByPrefixAction >(new FindFileByPrefixAction(args));
    };
    reg["find-file-by-tag"] = [](const std::vector< std::string > &args) -> std::unique_ptr< Action > {
      return std::unique_ptr< FindFileByTagAction >(new FindFileByTagAction(args));
    };
    reg["find-tag-by-name"] = [](const std::vector< std::string > &args) -> std::unique_ptr< Action > {
      return std::unique_ptr< FindTagByNameAction >(new FindTagByNameAction(args));
    };
    reg["find-tag-by-prefix"] = [](const std::vector< std::string > &args) -> std::unique_ptr< Action > {
      return std::unique_ptr< FindTagByPrefixAction >(new FindTagByPrefixAction(args));
    };
    reg["find-collection-by-name"] = [](const std::vector< std::string > &args) -> std::unique_ptr< Action > {
      return std::unique_ptr< FindCollectionByNameAction >(new FindCollectionByNameAction(args));
    };
    reg["find-collection-by-prefix"] = [](const std::vector< std::string > &args) -> std::unique_ptr< Action > {
      return std::unique_ptr< FindCollectionByPrefixAction >(new FindCollectionByPrefixAction(args));
    };
    reg["union-collections"] = [](const std::vector< std::string > &args) -> std::unique_ptr< Action > {
      return std::unique_ptr< UnionCollectionsAction >(new UnionCollectionsAction(args));
    };
    reg["intersect-collections"] = [](const std::vector< std::string > &args) -> std::unique_ptr< Action > {
      return std::unique_ptr< IntersectCollectionsAction >(new IntersectCollectionsAction(args));
    };
    reg["filter-by-tag"] = [](const std::vector< std::string > &args) -> std::unique_ptr< Action > {
      return std::unique_ptr< FilterByTagAction >(new FilterByTagAction(args));
    };
  }
}
