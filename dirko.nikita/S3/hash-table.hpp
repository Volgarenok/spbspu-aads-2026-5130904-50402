#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP
#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include "../common/list.hpp"
#include "../common/vector.hpp"

namespace dirko
{
  template < class Key, class Value, class Hash, class Equal >
  class HashTable
  {
  public:
    HashTable(size_t slots);
    HashTable(std::initializer_list< std::pair< Key, Value > > il);

    void add(Key k, Value v);
    void drop(Key k);
    Value get(Key k) const;
    bool has(Key k) const noexcept;
    void rehash(size_t slots);

    void clear() noexcept;
    size_t size() const noexcept;
    bool empty() const noexcept;
    void swap(HashTable &other) noexcept;

  private:
    Vector< List< std::pair< Key, Value > > * > data_;
    Hash hasher_;
    Equal comparator_;
    size_t slots_;
    size_t elements_;
  };
}

template < class Key, class Value, class Hash, class Equal >
dirko::HashTable< Key, Value, Hash, Equal >::HashTable(size_t slots):
  data_(),
  hasher_(Hash{}),
  comparator_(Equal{}),
  slots_(slots),
  elements_(0)
{
  data_.reserve(slots);
}

template < class Key, class Value, class Hash, class Equal >
dirko::HashTable< Key, Value, Hash, Equal >::HashTable(std::initializer_list< std::pair< Key, Value > > il):
  data_(),
  hasher_(Hash{}),
  comparator_(Equal{}),
  slots_(il.size()),
  elements_(0)
{
  data_.reserve(il.size());
  for (const std::pair< Key, Value > &v : il) {
    size_t id = hasher_(v.first) % slots_;
    data_[id].push_back(v.second);
  }
}

template < class Key, class Value, class Hash, class Equal >
void dirko::HashTable< Key, Value, Hash, Equal >::add(Key k, Value v)
{
  size_t id = hasher_(k) % slots_;
  data_[id].push_back(v);
  ++elements_;
}

template < class Key, class Value, class Hash, class Equal >
void dirko::HashTable< Key, Value, Hash, Equal >::drop(Key k)
{
  size_t id = hasher_(k) % slots_;
  List< std::pair< Key, Value > > *head = data_[id];
  for (LIter< std::pair< Key, Value > > v = head->begin(); v != head->end(); ++v) {
    if (comparator_(*v.first, k)) {
      head->erase(v++);
      --v;
    }
  }
}
void dirko::HashTable< Key, Value, Hash, Equal >::clear() noexcept
{
  for (size_t i = 0; i < slots_; ++i) {
    data_[i].clear();
  }
  elements_ = 0;
}
template < class Key, class Value, class Hash, class Equal >
size_t dirko::HashTable< Key, Value, Hash, Equal >::size() const noexcept
{
  return elements_;
}
template < class Key, class Value, class Hash, class Equal >
bool dirko::HashTable< Key, Value, Hash, Equal >::empty() const noexcept
{
  return !elements_;
}
template < class Key, class Value, class Hash, class Equal >
void dirko::HashTable< Key, Value, Hash, Equal >::swap(HashTable &other) noexcept
{
  data_.swap(other.data_);
  hasher_.swap(other.hasher_);
  comparator_.swap(other.comparator_);
  std::swap(slots_, other.slots_);
  std::swap(elements_, other.elements_);
}
#endif
