#ifndef MATVEEV_GAMEWORLD_PATHFINDING_HPP
#define MATVEEV_GAMEWORLD_PATHFINDING_HPP

#include "world.hpp"

#include "../common/list.hpp"

#include <cstddef>
#include <string>

namespace matveev
{
bool findShortestPath(const World& world, const std::string& from, const std::string& to,
  List< std::string >& path_out, unsigned long long& cost_out);

bool findItemRoute(const World& world, const std::string& from, const std::string& to,
  const List< std::string >& items, List< std::string >& path_out, unsigned long long& cost_out);

bool findTypeRoute(const World& world, const std::string& from, const std::string& to,
  std::size_t count, const std::string& type, List< std::string >& path_out, unsigned long long& cost_out);
}

#endif
