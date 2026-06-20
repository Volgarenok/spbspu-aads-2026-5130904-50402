#ifndef MATVEEV_GAMEWORLD_WORLD_HPP
#define MATVEEV_GAMEWORLD_WORLD_HPP

#include "location.hpp"
#include "hashFunctions.hpp"
#include "hash-table.hpp"

#include "../common/list.hpp"

#include <string>

namespace matveev
{
class World
{
public:
  World();

  void swap(World& other) noexcept;

  bool hasLocation(const std::string& name) const;
  const Location& location(const std::string& name) const;
  const List< std::string >& order() const noexcept;

  void createLocation(const std::string& name);
  void removeLocation(const std::string& name);
  void renameLocation(const std::string& name, const std::string& new_name);
  void clearLocation(const std::string& name);
  void mergeLocations(const std::string& new_name, const std::string& first, const std::string& second);

  void addItem(const std::string& location, const std::string& item, const std::string& type);
  void removeItem(const std::string& location, const std::string& item);
  void renameItem(const std::string& location, const std::string& item, const std::string& new_name);
  void moveItem(const std::string& item, const std::string& from, const std::string& to);
  bool findItem(const std::string& item, std::string& location_out) const;

  void connect(const std::string& first, const std::string& second, unsigned long long cost);
  void disconnect(const std::string& first, const std::string& second);
  bool connected(const std::string& first, const std::string& second) const;

private:
  HashTable< std::string, Location, StringHash, StringEqual > locations_;
  HashTable< std::string, std::string, StringHash, StringEqual > itemIndex_;
  List< std::string > order_;

  Location& locationRef(const std::string& name);
  void removeFromOrder(const std::string& name);
  void renameInOrder(const std::string& name, const std::string& new_name);
  void detachConnection(Location& owner, const std::string& neighbour);
  void renameConnection(Location& owner, const std::string& old_neighbour, const std::string& new_neighbour);
};
}

#endif
