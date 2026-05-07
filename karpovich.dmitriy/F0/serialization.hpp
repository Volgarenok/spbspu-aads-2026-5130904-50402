#ifndef SERIALIZATION_HPP
#define SERIALIZATION_HPP

#include <iosfwd>
#include "gameTypes.hpp"

namespace karpovich
{
  void serializeItem(const item_template_t &item, std::ostream &out);
  item_template_t deserializeItem(std::istream &in);

  void serializeLink(const scene_link_t &link, std::ostream &out);
  scene_link_t deserializeLink(std::istream &in);

  void serializeObject(const scene_object_t &obj, std::ostream &out);
  scene_object_t deserializeObject(std::istream &in);

  void serializeScene(const scene_t &scene, std::ostream &out);
  scene_t deserializeScene(std::istream &in);

  void serializeProject(const project_t &project, std::ostream &out);
  project_t deserializeProject(std::istream &in);

  void serializeSave(const save_state_t &state, std::ostream &out);
  save_state_t deserializeSave(std::istream &in);
}
#endif
