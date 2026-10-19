#ifndef HASHROBIN_HPP
#define HASHROBIN_HPP
#include <algorithm>
#include <stdexcept>
#include <functional>
#include <utility>
#include <memory>
#include <cstddef>
#include <initializer_list>
#include "hasher.hpp"
#include "vector.hpp"

namespace permyakov
{
  template< class Key, class Value >
  struct RobinNode
  {
    std::pair< Key, Value > value;
    size_t psl;
    bool engaged;
    RobinNode();
    RobinNode(Key k, Value v, size_t psl_);
    void swap(RobinNode &other);
  };

  template< class Key, class Value, class Hash, class Equal >
  class RobinIter;

  template< class Key, class Value, class Hash = permyakov::sipHasher< Key >, class Equal = std::equal_to< Key > >
  class RobinTable
  {
    Vector< RobinNode< Key, Value > > data;
    Hash hasher;
    Equal comparator;
    size_t length;
    size_t elements;
    friend class RobinIter< Key, Value, Hash, Equal >;
    public:
      RobinTable();
      explicit RobinTable(size_t length);
      RobinTable(std::initializer_list< std::pair< Key, Value > > list);

      bool has(Key k) const noexcept;
      void add(Key k, Value v);
      void drop(Key k);
      Value & get(Key k);
      const Value & get(Key k) const;
      void rehash(size_t length);

      size_t size() const noexcept;
      bool isEmpty() const noexcept;
      void clear() noexcept;
      void swap(RobinTable & other) noexcept;  
  };

  template< class Key, class Value, class Hash = permyakov::sipHasher< Key >, class Equal = std::equal_to< Key > >
  class RobinIter
  {
    Vector< RobinNode< Key, Value > > * data;
    size_t id;
    public:
      explicit RobinIter(RobinTable< Key, Value, Hash, Equal > & table);
      bool hasNext();
      void next();
      RobinIter &operator++();
      std::pair< Key, Value > & operator*();
  };
}

template< class Key, class Value >
permyakov::RobinNode< Key, Value >::RobinNode():
  value(),
  psl(0),
  engaged(false)
{}

template< class Key, class Value >
permyakov::RobinNode< Key, Value >::RobinNode(Key k, Value v, size_t psl_):
  value(std::pair< Key, Value > (k, v)),
  psl(psl_),
  engaged(true)
{}

template< class Key, class Value >
void permyakov::RobinNode< Key, Value >::swap(RobinNode &other)
{
  std::pair< Key, Value > value_ = value;
  size_t psl_ = psl;
  bool engaged_ = engaged;
  value = other.value;
  psl = other.psl;
  engaged = other.engaged;
  other.value = value_;
  other.psl = psl_;
  other.engaged = engaged_;
}

template< class Key, class Value, class Hash, class Equal >
permyakov::RobinTable< Key, Value, Hash, Equal >::RobinTable(size_t length):
  data(),
  hasher(Hash{}),
  comparator(Equal{}),
  length(length),
  elements(0)
{
  data.reserve(length);
  for (size_t i = 0; i < length; ++i) {
    data.pushBack(RobinNode< Key, Value >());
  }
}

template< class Key, class Value, class Hash, class Equal >
permyakov::RobinTable< Key, Value, Hash, Equal >::RobinTable():
  RobinTable(8)
{}

template< class Key, class Value, class Hash, class Equal >
permyakov::RobinTable< Key, Value, Hash, Equal >::RobinTable(std::initializer_list< std::pair< Key, Value > > list):
  RobinTable(list.size())
{
  data.reserve(list.size());
  for (const std::pair< Key, Value > & k : list) {
    add(k.first, k.second);
  }
}

template< class Key, class Value, class Hash, class Equal >
bool permyakov::RobinTable< Key, Value, Hash, Equal >::has(Key k) const noexcept
{
  if (data.isEmpty()) {
    return false;
  }
  size_t i = hasher(k) % length;
  size_t start = i;
  while (data[i].engaged) {
    if (comparator(k, data[i].value.first)) {
      return true;
    }
    i = (i + 1) % length;
    if (i == start) {
      return false;
    }
  }
  return false;
}

template< class Key, class Value, class Hash, class Equal >
void permyakov::RobinTable< Key, Value, Hash, Equal >::add(Key k, Value v)
{
  if (!data.isEmpty()) {
    size_t i = hasher(k) % length;
    size_t start = i;
    while (data[i].engaged) {
      if (comparator(k, data[i].value.first)) {
        data[i].value.second = v;
        return;
      }
      i = (i + 1) % length;
      if (i == start) {
        break;
      }
    }
  }
  if (elements >= length * 0.9) {
    rehash(!length ? 16 : length * 2);
  }
  size_t i = hasher(k) % length;
  RobinNode< Key, Value > node_ = RobinNode< Key, Value >(k, v, 0);
  while (data[i].engaged) {
    if (data[i].psl < node_.psl) {
      node_.swap(data[i]);
    }
    i = (i + 1) % length;
    node_.psl++;
  }
  node_.swap(data[i]);
  ++elements; 
}

template< class Key, class Value, class Hash, class Equal >
void permyakov::RobinTable< Key, Value, Hash, Equal >::drop(Key k)
{
  if (data.isEmpty()) {
    return;
  }
  size_t i = hasher(k) % length;
  size_t start = i;
  while (data[i].engaged) {
    if (comparator(k, data[i].value.first)) {
      size_t cur = i;
      size_t next = (cur + 1) % length;
      while (data[next].engaged && data[next].psl > 0) {
        data[cur].value = std::move(data[next].value);
        data[cur].psl = data[next].psl - 1;
        cur = next;
        next = (next + 1) % length;
      }
      data[cur].psl = 0;
      data[cur].engaged = false;
      --elements;
      return;
    }
    i = (i + 1) % length;
    if (i == start) {
      throw std::invalid_argument("None key");
    }
  }
}

template< class Key, class Value, class Hash, class Equal >
Value &permyakov::RobinTable< Key, Value, Hash, Equal >::get(Key k)
{
  size_t i = hasher(k) % length;
  size_t start = i;
  while (data[i].engaged) {
    if (comparator(k, data[i].value.first)) {
      return data[i].value.second;
    }
    i = (i + 1) % length;
    if (i == start) {
      throw std::invalid_argument("None key");
    }
  }
  throw std::invalid_argument("None key");
}

template< class Key, class Value, class Hash, class Equal >
const Value &permyakov::RobinTable< Key, Value, Hash, Equal >::get(Key k) const
{
  size_t i = hasher(k) % length;
  size_t start = i;
  while (data[i].engaged) {
    if (comparator(k, data[i].value.first)) {
      return data[i].value.second;
    }
    i = (i + 1) % length;
    if (i == start) {
      throw std::invalid_argument("None key");
    }
  }
  throw std::invalid_argument("None key");
}


template< class Key, class Value, class Hash, class Equal >
void permyakov::RobinTable< Key, Value, Hash, Equal >::rehash(size_t len)
{
  RobinTable< Key, Value, Hash, Equal > cpy(len);
  for(size_t i = 0; i < length; ++i) {
    if(data[i].engaged) {
      cpy.add(data[i].value.first, data[i].value.second);
    }
  }
  swap(cpy);
}

template< class Key, class Value, class Hash, class Equal >
size_t permyakov::RobinTable< Key, Value, Hash, Equal >::size() const noexcept
{
  return elements;
}

template< class Key, class Value, class Hash, class Equal >
bool permyakov::RobinTable< Key, Value, Hash, Equal >::isEmpty() const noexcept
{
  return !elements;
}

template< class Key, class Value, class Hash, class Equal >
void permyakov::RobinTable< Key, Value, Hash, Equal >::clear() noexcept
{
  for (size_t i = 0; i < length; ++i) {
    data[i].engaged = false;
  }
  elements = 0;
}

template< class Key, class Value, class Hash, class Equal >
void permyakov::RobinTable< Key, Value, Hash, Equal >::swap(RobinTable & other) noexcept
{
  data.swap(other.data);
  std::swap(hasher, other.hasher);
  std::swap(comparator, other.comparator);
  std::swap(length, other.length);
  std::swap(elements, other.elements);
}

template< class Key, class Value, class Hash, class Equal >
permyakov::RobinIter< Key, Value, Hash, Equal >::RobinIter(permyakov::RobinTable< Key, Value, Hash, Equal > & table):
  data(&table.data),
  id(0)
{
  if (table.isEmpty()) {
    throw std::logic_error("Empty table");
  }
  while (id < data->size() && !(*data)[id].engaged) {
    ++id;
  }
}

template< class Key, class Value, class Hash, class Equal >
bool permyakov::RobinIter< Key, Value, Hash, Equal >::hasNext()
{
  size_t j = id + 1;
  while (j < data->size() && !(*data)[j].engaged) {
    ++j;
  }
  return j < data->size();
}

template< class Key, class Value, class Hash, class Equal >
void permyakov::RobinIter< Key, Value, Hash, Equal >::next()
{
  if (id < data->size()) {
    ++id;
  }
  while (id < data->size() && !(*data)[id].engaged) {
    ++id;
  }
}

template< class Key, class Value, class Hash, class Equal >
permyakov::RobinIter< Key, Value, Hash, Equal > & permyakov::RobinIter< Key, Value, Hash, Equal >::operator++()
{
  next();
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
std::pair< Key, Value > & permyakov::RobinIter< Key, Value, Hash, Equal >::operator*()
{
  return (*data)[id].value;
}

#endif
