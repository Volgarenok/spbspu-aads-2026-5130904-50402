#include "serialization.hpp"
#include <iostream>

std::string readLine(std::istream &in)
{
  std::string line;
  if (!std::getline(in, line)) {
    throw std::runtime_error("Unexpected end of input");
  }
  return line;
}

void checkPrefix(const std::string &line, const std::string &expected)
{
  if (line.size() < expected.size() || line.substr(0, expected.size()) != expected) {
    throw std::runtime_error("Invalid format: expected '" + expected + "'");
  }
}

std::string extractToken(const std::string &line, size_t &pos)
{
  while (pos < line.size() && line[pos] == ' ') {
    ++pos;
  }
  size_t start = pos;
  while (pos < line.size() && line[pos] != ' ') {
    ++pos;
  }
  return line.substr(start, pos - start);
}

std::string extractQuoted(const std::string &line, size_t &pos)
{
  while (pos < line.size() && line[pos] == ' ') {
    ++pos;
  }
  if (pos >= line.size() || line[pos] != '"') {
    return "";
  }
  ++pos;
  size_t start = pos;
  while (pos < line.size() && line[pos] != '"') {
    ++pos;
  }
  std::string res = line.substr(start, pos - start);
  if (pos < line.size()) {
    ++pos;
  }
  return res;
}

void karpovich::serializeItem(const item_template_t &item, std::ostream &out)
{
  out << "ITEM " << item.key_ << " " << item.type_ << " " << item.name_ << "\n";
}

karpovich::item_template_t karpovich::deserializeItem(std::istream &in)
{
  std::string line = readLine(in);
  checkPrefix(line, "ITEM ");
  size_t pos = 5;
  std::string key = extractToken(line, pos);
  std::string type = extractToken(line, pos);
  std::string name = extractToken(line, pos);
  return item_template_t{key, type, name};
}

void karpovich::serializeLink(const scene_link_t &link, std::ostream &out)
{
  out << "LINK " << link.target_ << " \"" << link.description_ << "\" " << link.condition_ << "\n";
}

karpovich::scene_link_t karpovich::deserializeLink(std::istream &in)
{
  std::string line = readLine(in);
  checkPrefix(line, "LINK ");
  size_t pos = 5;
  scene_link_t res;
  res.target_ = extractToken(line, pos);
  res.description_ = extractQuoted(line, pos);
  res.condition_ = extractToken(line, pos);
  return res;
}

void karpovich::serializeObject(const scene_object_t &obj, std::ostream &out)
{
  out << "OBJ " << obj.key_ << " \"" << obj.action_ << "\" " << obj.gives_ << " " << obj.requires_ << "\n";
}

void karpovich::serializeScene(const scene_t &scene, std::ostream &out)
{
  out << "SCENE " << scene.id_ << " \"" << scene.description_ << "\"\n";
  for (size_t i = 0; i < scene.links_.getSize(); ++i) {
    serializeLink(scene.links_[i], out);
  }
  out << "END_LINKS\n";
  for (size_t i = 0; i < scene.objects_.getSize(); ++i) {
    serializeObject(scene.objects_[i], out);
  }
  out << "END_OBJECTS\n";
  out << "END_SCENE\n";
}

void karpovich::serializeProject(const project_t &project, std::ostream &out)
{
  out << "PROJECT \"" << project.title_ << "\" " << project.start_scene_id_ << "\n";
  for (karpovich::HashTable< std::string, scene_t >::HCIter it = project.scenes_.cbegin(); it != project.scenes_.cend();
       ++it) {
    serializeScene((*it).second, out);
  }
  out << "END_PROJECT\n";
}

void karpovich::serializeSave(const save_state_t &state, std::ostream &out)
{
  out << "SAVE " << state.current_scene_id_ << "\n";
  for (karpovich::HashTable< std::string, int >::HCIter it = state.inventory_.cbegin(); it != state.inventory_.cend();
       ++it) {
    out << "INV " << (*it).first << " " << (*it).second << "\n";
  }
  out << "END_SAVE\n";
}
