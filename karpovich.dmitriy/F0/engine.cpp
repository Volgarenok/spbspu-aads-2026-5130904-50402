#include "engine.hpp"
#include <iostream>
#include "gameTypes.hpp"
#include "serialization.hpp"

karpovich::Engine::Engine():
  item_db_(16),
  active_project_(),
  game_state_(),
  connected_dbs_(),
  known_projects_(),
  mode_("editor"),
  project_loaded_(false),
  project_filename_(),
  command_table_(32)
{
  initCommandTable();
}

void karpovich::Engine::initCommandTable()
{
  command_table_.add("create-item", &Engine::cmdCreateItem);
  command_table_.add("remove-item", &Engine::cmdRemoveItem);
  command_table_.add("add-asset-db", &Engine::cmdAddAssetDb);
  command_table_.add("remove-asset-db", &Engine::cmdRemoveAssetDb);
  command_table_.add("show-db", &Engine::cmdShowDb);
  command_table_.add("create-game", &Engine::cmdCreateGame);
  command_table_.add("load-project", &Engine::cmdLoadProject);
  command_table_.add("save-project", &Engine::cmdSaveProject);
  command_table_.add("validate", &Engine::cmdValidate);
  command_table_.add("create-scene", &Engine::cmdCreateScene);
  command_table_.add("remove-scene", &Engine::cmdRemoveScene);
  command_table_.add("link-scene", &Engine::cmdLinkScene);
  command_table_.add("unlink-scene", &Engine::cmdUnlinkScene);
  command_table_.add("add-object", &Engine::cmdAddObject);
  command_table_.add("remove-object", &Engine::cmdRemoveObject);
  command_table_.add("set-interact", &Engine::cmdSetInteract);
  command_table_.add("start", &Engine::cmdStart);
  command_table_.add("look", &Engine::cmdLook);
  command_table_.add("interact", &Engine::cmdInteract);
  command_table_.add("choice", &Engine::cmdChoice);
  command_table_.add("show-inv", &Engine::cmdShowInv);
  command_table_.add("save-game", &Engine::cmdSaveGame);
  command_table_.add("load-game", &Engine::cmdLoadGame);
  command_table_.add("mode", &Engine::cmdMode);
  command_table_.add("faq", &Engine::cmdFaq);
}

void karpovich::Engine::tokenize(const std::string &line, Vector< std::string > &tokens) const
{
  std::string current;
  bool in_quotes = false;
  for (size_t i = 0; i < line.size(); ++i) {
    char c = line[i];
    if (c == '"') {
      in_quotes = !in_quotes;
    } else if (c == ' ' && !in_quotes) {
      if (!current.empty()) {
        tokens.pushBack(current);
        current.clear();
      }
    } else {
      current += c;
    }
  }
  if (!current.empty()) {
    tokens.pushBack(current);
  }
}

void karpovich::Engine::cmdFaq(const Vector< std::string > &args)
{
  if (args.getSize() != 1) {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }
  std::cout << "<HELP: create-game, load-project, start, interact, choice, show-inv, "
               "save-game, load-game, mode, faq, create-item, remove-item, add-asset-db, "
               "remove-asset-db, show-db, create-scene, remove-scene, link-scene, "
               "unlink-scene, add-object, remove-object, set-interact, validate, save-project>\n";
}
