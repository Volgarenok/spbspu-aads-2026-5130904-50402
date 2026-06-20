#ifndef MATVEEV_GAMEWORLD_LOCATION_HPP
#define MATVEEV_GAMEWORLD_LOCATION_HPP

#include "../common/list.hpp"

#include <string>

namespace matveev
{
struct Item
{
  Item();
  Item(const std::string& name_value, const std::string& type_value);

  std::string name;
  std::string type;
};

inline Item::Item():
  name(),
  type()
{}

inline Item::Item(const std::string& name_value, const std::string& type_value):
  name(name_value),
  type(type_value)
{}

struct Connection
{
  Connection();
  Connection(const std::string& to_value, unsigned long long cost_value);

  std::string to;
  unsigned long long cost;
};

inline Connection::Connection():
  to(),
  cost(0)
{}

inline Connection::Connection(const std::string& to_value, unsigned long long cost_value):
  to(to_value),
  cost(cost_value)
{}

struct Location
{
  Location();
  explicit Location(const std::string& name_value);

  std::string name;
  List< Item > items;
  List< Connection > connections;
};

inline Location::Location():
  name(),
  items(),
  connections()
{}

inline Location::Location(const std::string& name_value):
  name(name_value),
  items(),
  connections()
{}
}

#endif
