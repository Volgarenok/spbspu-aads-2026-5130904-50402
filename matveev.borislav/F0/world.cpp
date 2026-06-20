#include "world.hpp"

#include <stdexcept>

namespace
{
template< class T >
void appendBack(matveev::List< T >& list, const T& value)
{
  matveev::LIter< T > prev = list.beforeBegin();
  matveev::LIter< T > it = list.begin();

  while (it != list.end())
  {
    ++prev;
    ++it;
  }

  list.insertAfter(prev, value);
}

bool itemsContain(const matveev::List< matveev::Item >& items, const std::string& name)
{
  for (matveev::LCIter< matveev::Item > it = items.begin(); it != items.end(); ++it)
  {
    if (it->name == name)
    {
      return true;
    }
  }

  return false;
}
}

matveev::World::World():
  locations_(16),
  itemIndex_(16),
  order_()
{}

void matveev::World::swap(World& other) noexcept
{
  locations_.swap(other.locations_);
  itemIndex_.swap(other.itemIndex_);
  order_.swap(other.order_);
}

bool matveev::World::hasLocation(const std::string& name) const
{
  return locations_.has(name);
}

const matveev::Location& matveev::World::location(const std::string& name) const
{
  return locations_.at(name);
}

const matveev::List< std::string >& matveev::World::order() const noexcept
{
  return order_;
}

matveev::Location& matveev::World::locationRef(const std::string& name)
{
  return locations_.at(name);
}

void matveev::World::removeFromOrder(const std::string& name)
{
  LIter< std::string > prev = order_.beforeBegin();
  LIter< std::string > it = order_.begin();

  while (it != order_.end())
  {
    if (*it == name)
    {
      order_.eraseAfter(prev);
      return;
    }

    ++prev;
    ++it;
  }
}

void matveev::World::renameInOrder(const std::string& name, const std::string& new_name)
{
  for (LIter< std::string > it = order_.begin(); it != order_.end(); ++it)
  {
    if (*it == name)
    {
      *it = new_name;
      return;
    }
  }
}

void matveev::World::detachConnection(Location& owner, const std::string& neighbour)
{
  LIter< Connection > prev = owner.connections.beforeBegin();
  LIter< Connection > it = owner.connections.begin();

  while (it != owner.connections.end())
  {
    if (it->to == neighbour)
    {
      owner.connections.eraseAfter(prev);
      return;
    }

    ++prev;
    ++it;
  }
}

void matveev::World::renameConnection(Location& owner, const std::string& old_neighbour,
  const std::string& new_neighbour)
{
  for (LIter< Connection > it = owner.connections.begin(); it != owner.connections.end(); ++it)
  {
    if (it->to == old_neighbour)
    {
      it->to = new_neighbour;
      return;
    }
  }
}

void matveev::World::createLocation(const std::string& name)
{
  if (locations_.has(name))
  {
    throw std::logic_error("location already exists");
  }

  locations_.add(name, Location(name));
  appendBack(order_, name);
}

void matveev::World::removeLocation(const std::string& name)
{
  const Location& target = location(name);

  for (LCIter< Connection > it = target.connections.begin(); it != target.connections.end(); ++it)
  {
    detachConnection(locationRef(it->to), name);
  }

  for (LCIter< Item > it = target.items.begin(); it != target.items.end(); ++it)
  {
    itemIndex_.drop(it->name);
  }

  removeFromOrder(name);
  locations_.drop(name);
}

void matveev::World::renameLocation(const std::string& name, const std::string& new_name)
{
  if (!locations_.has(name))
  {
    throw std::out_of_range("location not found");
  }

  if (locations_.has(new_name))
  {
    throw std::logic_error("target location already exists");
  }

  Location updated = locations_.at(name);
  updated.name = new_name;

  for (LIter< Connection > it = updated.connections.begin(); it != updated.connections.end(); ++it)
  {
    renameConnection(locationRef(it->to), name, new_name);
  }

  for (LIter< Item > it = updated.items.begin(); it != updated.items.end(); ++it)
  {
    itemIndex_.at(it->name) = new_name;
  }

  renameInOrder(name, new_name);
  locations_.drop(name);
  locations_.add(new_name, updated);
}

void matveev::World::clearLocation(const std::string& name)
{
  Location& target = locationRef(name);

  for (LIter< Item > it = target.items.begin(); it != target.items.end(); ++it)
  {
    itemIndex_.drop(it->name);
  }

  target.items.clear();
}

void matveev::World::addItem(const std::string& location, const std::string& item, const std::string& type)
{
  Location& target = locationRef(location);

  if (itemIndex_.has(item))
  {
    throw std::logic_error("item name already used");
  }

  appendBack(target.items, Item(item, type));
  itemIndex_.add(item, location);
}

void matveev::World::removeItem(const std::string& location, const std::string& item)
{
  Location& target = locationRef(location);

  LIter< Item > prev = target.items.beforeBegin();
  LIter< Item > it = target.items.begin();

  while (it != target.items.end())
  {
    if (it->name == item)
    {
      target.items.eraseAfter(prev);
      itemIndex_.drop(item);
      return;
    }

    ++prev;
    ++it;
  }

  throw std::out_of_range("item not in location");
}

void matveev::World::renameItem(const std::string& location, const std::string& item, const std::string& new_name)
{
  Location& target = locationRef(location);

  if (!itemsContain(target.items, item))
  {
    throw std::out_of_range("item not in location");
  }

  if (itemIndex_.has(new_name))
  {
    throw std::logic_error("item name already used");
  }

  for (LIter< Item > it = target.items.begin(); it != target.items.end(); ++it)
  {
    if (it->name == item)
    {
      it->name = new_name;
      break;
    }
  }

  itemIndex_.drop(item);
  itemIndex_.add(new_name, location);
}

void matveev::World::moveItem(const std::string& item, const std::string& from, const std::string& to)
{
  Location& source = locationRef(from);

  if (!locations_.has(to))
  {
    throw std::out_of_range("location not found");
  }

  LIter< Item > prev = source.items.beforeBegin();
  LIter< Item > it = source.items.begin();

  while (it != source.items.end() && it->name != item)
  {
    ++prev;
    ++it;
  }

  if (it == source.items.end())
  {
    throw std::out_of_range("item not in location");
  }

  if (from == to)
  {
    return;
  }

  Item moved = *it;
  source.items.eraseAfter(prev);

  appendBack(locationRef(to).items, moved);
  itemIndex_.at(item) = to;
}

bool matveev::World::findItem(const std::string& item, std::string& location_out) const
{
  if (!itemIndex_.has(item))
  {
    return false;
  }

  location_out = itemIndex_.at(item);
  return true;
}

bool matveev::World::connected(const std::string& first, const std::string& second) const
{
  if (!locations_.has(first))
  {
    return false;
  }

  const Location& owner = locations_.at(first);

  for (LCIter< Connection > it = owner.connections.begin(); it != owner.connections.end(); ++it)
  {
    if (it->to == second)
    {
      return true;
    }
  }

  return false;
}

void matveev::World::connect(const std::string& first, const std::string& second, unsigned long long cost)
{
  if (first == second)
  {
    throw std::logic_error("self connection");
  }

  Location& a = locationRef(first);

  if (!locations_.has(second))
  {
    throw std::out_of_range("location not found");
  }

  if (connected(first, second))
  {
    throw std::logic_error("already connected");
  }

  Location& b = locationRef(second);
  appendBack(a.connections, Connection(second, cost));
  appendBack(b.connections, Connection(first, cost));
}

void matveev::World::disconnect(const std::string& first, const std::string& second)
{
  Location& a = locationRef(first);

  if (!locations_.has(second))
  {
    throw std::out_of_range("location not found");
  }

  if (!connected(first, second))
  {
    throw std::logic_error("not connected");
  }

  detachConnection(a, second);
  detachConnection(locationRef(second), first);
}

void matveev::World::mergeLocations(const std::string& new_name, const std::string& first, const std::string& second)
{
  if (locations_.has(new_name))
  {
    throw std::logic_error("target location already exists");
  }

  if (first == second)
  {
    throw std::logic_error("cannot merge a location with itself");
  }

  if (!locations_.has(first) || !locations_.has(second))
  {
    throw std::out_of_range("location not found");
  }

  using NeighbourCosts = HashTable< std::string, unsigned long long, StringHash, StringEqual >;
  NeighbourCosts neighbours(16);

  const Location& source_a = locations_.at(first);
  for (LCIter< Connection > it = source_a.connections.begin(); it != source_a.connections.end(); ++it)
  {
    if (it->to == second)
    {
      continue;
    }

    if (neighbours.has(it->to))
    {
      unsigned long long current = neighbours.at(it->to);
      neighbours.at(it->to) = (it->cost < current) ? it->cost : current;
    }
    else
    {
      neighbours.add(it->to, it->cost);
    }
  }

  const Location& source_b = locations_.at(second);
  for (LCIter< Connection > it = source_b.connections.begin(); it != source_b.connections.end(); ++it)
  {
    if (it->to == first)
    {
      continue;
    }

    if (neighbours.has(it->to))
    {
      unsigned long long current = neighbours.at(it->to);
      neighbours.at(it->to) = (it->cost < current) ? it->cost : current;
    }
    else
    {
      neighbours.add(it->to, it->cost);
    }
  }

  Location merged(new_name);

  for (LCIter< Item > it = source_a.items.begin(); it != source_a.items.end(); ++it)
  {
    appendBack(merged.items, *it);
    itemIndex_.at(it->name) = new_name;
  }

  for (LCIter< Item > it = source_b.items.begin(); it != source_b.items.end(); ++it)
  {
    appendBack(merged.items, *it);
    itemIndex_.at(it->name) = new_name;
  }

  for (NeighbourCosts::ConstIterator it = neighbours.cbegin(); it != neighbours.cend(); ++it)
  {
    appendBack(merged.connections, Connection(it->key, it->value));

    Location& neighbour = locationRef(it->key);
    detachConnection(neighbour, first);
    detachConnection(neighbour, second);
    appendBack(neighbour.connections, Connection(new_name, it->value));
  }

  removeFromOrder(first);
  removeFromOrder(second);
  locations_.drop(first);
  locations_.drop(second);

  locations_.add(new_name, merged);
  appendBack(order_, new_name);
}
