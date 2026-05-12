#include "engine.hpp"
#include <fstream>
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

void karpovich::Engine::processCommand(const std::string &line)
{
  Vector< std::string > tokens;
  tokenize(line, tokens);
  if (tokens.getSize() > 0) {
    dispatch(tokens);
  }
}

void karpovich::Engine::dispatch(const Vector< std::string > &args)
{
  const std::string &cmd = args[0];
  try {
    if (command_table_.has(cmd)) {
      cmd_handler_t handler = command_table_.get(cmd);
      (this->*handler)(args);
    } else {
      std::cout << "<UNKNOWN COMMAND>\n";
    }
  } catch (const std::exception &) {
    std::cout << "<INVALID COMMAND>\n";
  }
}

bool karpovich::Engine::checkMode(const std::string &required_mode) const
{
  if (mode_ != required_mode) {
    std::cout << "<INVALID MODE>\n";
    return false;
  }
  return true;
}

bool karpovich::Engine::isProjectLoaded() const
{
  if (!project_loaded_) {
    std::cout << "<ERROR: NO PROJECT LOADED>\n";
    return false;
  }
  return true;
}

bool karpovich::Engine::isGameRunning() const
{
  if (!game_state_.running_) {
    std::cout << "<NOT IN GAME>\n";
    return false;
  }
  return true;
}

karpovich::scene_t &karpovich::Engine::getScene(const std::string &id)
{
  return const_cast< scene_t & >(const_cast< const Engine * >(this)->getScene(id));
}

const karpovich::scene_t &karpovich::Engine::getScene(const std::string &id) const
{
  return active_project_.scenes_.get(id);
}

void karpovich::Engine::cmdMode(const Vector< std::string > &args)
{
  if (args.getSize() < 2) {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }
  if (args[1] != "editor" && args[1] != "game") {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }
  mode_ = args[1];
  std::string mode_str = args[1];
  if (mode_str.size() > 0) {
    mode_str[0] = std::toupper(mode_str[0]);
  }
  std::cout << "<MODE: " << mode_str << ">\n";
}

void karpovich::Engine::cmdLoadGame(const Vector< std::string > &args)
{
  if (args.getSize() < 2) {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }
  std::ifstream file(args[1] + ".save");
  if (!file.is_open()) {
    std::cout << "<ERROR: SAVE FILE NOT FOUND>\n";
    return;
  }
  if (!isProjectLoaded()) {
    return;
  }
  game_state_ = deserializeSave(file);
  game_state_.running_ = true;
  std::cout << "<GAME LOADED: " << args[1] << ">\n";
  std::cout << "<SCENE: " << game_state_.current_scene_id_ << ">\n";
}

void karpovich::Engine::cmdSaveGame(const Vector< std::string > &args)
{
  if (!isGameRunning() || args.getSize() < 2) {
    std::cout << "<ERROR: CANNOT SAVE>\n";
    return;
  }
  std::ofstream file(args[1] + ".save");
  if (!file.is_open()) {
    std::cout << "<ERROR: CANNOT SAVE>\n";
    return;
  }
  serializeSave(game_state_, file);
  std::cout << "<GAME SAVED: " << args[1] << ">\n";
}

void karpovich::Engine::cmdSaveProject(const Vector< std::string > &)
{
  if (!isProjectLoaded()) {
    return;
  }
  std::ofstream file(project_filename_ + ".dat");
  if (!file.is_open()) {
    std::cout << "<ERROR: WRITE PERMISSION DENIED>\n";
    return;
  }
  serializeProject(active_project_, file);
  size_t scenes = 0;
  size_t links = 0;
  karpovich::HashTable< std::string, scene_t >::HIter it = active_project_.scenes_.begin();
  karpovich::HashTable< std::string, scene_t >::HIter end_it = active_project_.scenes_.end();
  for (; it != end_it; ++it) {
    ++scenes;
    links += (*it).second.links_.getSize();
  }
  std::cout << "<PROJECT SAVED>\n";
  std::cout << "<SCENES: " << scenes << ", LINKS: " << links << ">\n";
}

void karpovich::Engine::cmdCreateGame(const Vector< std::string > &args)
{
  if (args.getSize() < 3) {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }
  for (size_t i = 0; i < known_projects_.getSize(); ++i) {
    if (known_projects_[i] == args[1]) {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }
  }
  known_projects_.pushBack(args[1]);
  std::ofstream file(args[1] + ".dat");
  file.close();
  std::cout << "<PROJECT CREATED: " << args[1] << ".dat>\n";
}

void karpovich::Engine::cmdLoadProject(const Vector< std::string > &args)
{
  if (args.getSize() < 2) {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }
  std::ifstream file(args[1] + ".dat");
  if (!file.is_open()) {
    std::cout << "<ERROR: FILE NOT FOUND>\n";
    return;
  }
  active_project_ = deserializeProject(file);
  project_filename_ = args[1];
  project_loaded_ = true;
  game_state_ = save_state_t{};
  std::cout << "<PROJECT LOADED: " << args[1] << ">\n";
  std::cout << "<SCENES: " << active_project_.scenes_.size() << ", ITEMS: 0>\n";
}

void karpovich::Engine::printSceneInfo(const scene_t &scene) const
{
  std::cout << "<" << scene.description_ << ">\n";
  std::string objects;
  for (size_t i = 0; i < scene.objects_.getSize(); ++i) {
    if (i > 0) {
      objects += ", ";
    }
    objects += scene.objects_[i].key_;
  }
  std::cout << "<OBJECTS: " << (objects.empty() ? "none" : objects) << ">\n";
  std::cout << "<CHOICES: ";
  for (size_t i = 0; i < scene.links_.getSize(); ++i) {
    if (i > 0) {
      std::cout << ", ";
    }
    std::cout << i << ": \"" << scene.links_[i].description_ << "\"";
  }
  std::cout << ">\n";
}

void karpovich::Engine::cmdCreateItem(const Vector< std::string > &args)
{
  if (args.getSize() < 4) {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }
  if (item_db_.has(args[1])) {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }
  item_db_.add(args[1], item_template_t{args[1], args[2], args[3]});
  std::cout << "<ITEM CREATED: " << args[1] << ">\n";
}

void karpovich::Engine::cmdRemoveItem(const Vector< std::string > &args)
{
  if (args.getSize() < 2) {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }
  if (!item_db_.has(args[1])) {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }
  item_db_.drop(args[1]);
  std::cout << "<ITEM REMOVED: " << args[1] << ">\n";
}

void karpovich::Engine::cmdAddAssetDb(const Vector< std::string > &args)
{
  if (args.getSize() < 2) {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }
  std::ifstream file(args[1]);
  if (!file.is_open()) {
    std::cout << "<ERROR: FILE NOT FOUND>\n";
    return;
  }
  size_t count = 0;
  std::string line;
  while (std::getline(file, line)) {
    if (line.empty() || line[0] == '#') {
      continue;
    }
    size_t space1 = line.find(' ');
    if (space1 == std::string::npos) {
      continue;
    }
    size_t space2 = line.find(' ', space1 + 1);
    if (space2 == std::string::npos) {
      continue;
    }

    std::string key = line.substr(0, space1);
    std::string type = line.substr(space1 + 1, space2 - space1 - 1);
    size_t name_start = space2 + 1;
    while (name_start < line.size() && line[name_start] == ' ') {
      ++name_start;
    }
    std::string name = line.substr(name_start);

    if (!key.empty() && !type.empty() && !name.empty() && !item_db_.has(key)) {
      item_db_.add(key, item_template_t{key, type, name});
      ++count;
    }
  }
  connected_dbs_.pushBack(args[1]);
  std::cout << "<ASSET DB CONNECTED: " << count << " items loaded>\n";
}
