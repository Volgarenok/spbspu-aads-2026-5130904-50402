#include "serialization.hpp"
#include <iostream>

void karpovich::serializeItem(const item_template_t &item, std::ostream &out)
{
  out << "ITEM " << item.key_ << " " << item.type_ << " " << item.name_ << "\n";
}

void karpovich::serializeLink(const scene_link_t &link, std::ostream &out)
{
  out << "LINK " << link.target_ << " \"" << link.description_ << "\" " << link.condition_ << "\n";
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
