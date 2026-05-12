#ifndef GAME_TYPES_HPP
#define GAME_TYPES_HPP

#include <string>
#include "../common/Vector.hpp"
#include "../common/hashTable.hpp"

namespace karpovich
{

  struct item_template_t
  {
    std::string key_;
    std::string type_;
    std::string name_;
  };

  struct scene_link_t
  {
    std::string target_;
    std::string description_;
    std::string condition_;
  };

  struct scene_object_t
  {
    std::string key_;
    std::string action_;
    std::string gives_;
    std::string requires_;
  };

  struct scene_t
  {
    std::string id_;
    std::string description_;
    Vector< scene_link_t > links_;
    Vector< scene_object_t > objects_;
  };

  struct project_t
  {
    project_t();
    std::string title_;
    std::string start_scene_id_;
    HashTable< std::string, scene_t > scenes_;
  };

  struct save_state_t
  {
    save_state_t();
    std::string current_scene_id_;
    HashTable< std::string, int > inventory_;
  };

}

karpovich::save_state_t::save_state_t():
  current_scene_id_(),
  inventory_(16)
{}

karpovich::project_t::project_t():
  title_(),
  start_scene_id_(),
  scenes_(16)
{}

#endif
