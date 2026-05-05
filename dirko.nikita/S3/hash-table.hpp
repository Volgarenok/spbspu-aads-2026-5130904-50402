#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP
#include <cstddef>
#include <functional>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include "../common/Vector.hpp"
#include "../common/list.hpp"
#include "hasher.hpp"

namespace dirko
{
  template < class Key, class Value, class Hash, class Equal >
  class HTIter;
  template < class Key, class Value, class Hash, class Equal >
  class HTCIter;
  template < class Key, class Value, class Hash = dirko::SipHasher< Key >, class Equal = std::equal_to< Key > >
  class HashTable
  {
  public:
    explicit HashTable(size_t slots);
    HashTable(std::initializer_list< std::pair< Key, Value > > il);
    HashTable(const HashTable &other);
    HashTable(HashTable &&other) noexcept;
    HashTable &operator=(const HashTable &other);
    HashTable &operator=(HashTable &&other) noexcept;

    ~HashTable();

    void add(Key k, Value v);
    void drop(Key k);
    Value get(Key k);
    const Value get(Key k) const;
    bool has(Key k) const noexcept;
    void rehash(size_t slots);

    using HTIt = HTIter< Key, Value, Hash, Equal >;
    using HTCIt = HTCIter< Key, Value, Hash, Equal >;
    HTIt begin() noexcept;
    HTIt end() noexcept;
    HTCIt cbegin() const noexcept;
    HTCIt cend() const noexcept;

    void clear() noexcept;
    size_t size() const noexcept;
    bool empty() const noexcept;
    void swap(HashTable &other) noexcept;

  private:
    Vector< List< std::pair< Key, Value > > > data_;
    Hash hasher_;
    Equal comparator_;
    size_t slots_;
    size_t elements_;
    friend class HTIter< Key, Value, Hash, Equal >;
    friend class HTCIter< Key, Value, Hash, Equal >;
  };
  template < class Key, class Value, class Hash, class Equal >
  class HTIter
  {
  public:
    HTIter(HashTable< Key, Value, Hash, Equal > *, LIter< std::pair< Key, Value > >, size_t);
    std::pair< Key, Value > &operator*();
    HTIter &operator++();
    HTIter &operator--();
    HTIter operator++(int);
    HTIter operator--(int);
    bool operator==(const HTIter< Key, Value, Hash, Equal > &) const;
    bool operator!=(const HTIter< Key, Value, Hash, Equal > &) const;

  private:
    HashTable< Key, Value, Hash, Equal > *table_;
    LIter< std::pair< Key, Value > > curr_;
    size_t slot_;
  };
  template < class Key, class Value, class Hash, class Equal >
  class HTCIter
  {
  public:
    HTCIter(HashTable< Key, Value, Hash, Equal > *, LCIter< std::pair< Key, Value > >, size_t);
    const std::pair< Key, Value > &operator*() const;
    HTCIter &operator++();
    HTCIter &operator--();
    HTCIter operator++(int);
    HTCIter operator--(int);
    bool operator==(const HTCIter< Key, Value, Hash, Equal > &) const;
    bool operator!=(const HTCIter< Key, Value, Hash, Equal > &) const;

  private:
    HashTable< Key, Value, Hash, Equal > *table_;
    LCIter< std::pair< Key, Value > > curr_;
    size_t slot_;
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
  for (size_t i = 0; i < slots_; ++i) {
    data_.pushBack(List< std::pair< Key, Value > >());
  }
}

template < class Key, class Value, class Hash, class Equal >
dirko::HashTable< Key, Value, Hash, Equal >::HashTable(std::initializer_list< std::pair< Key, Value > > il):
  HashTable(il.size())
{
  for (const std::pair< Key, Value > &v : il) {
    add(v.first, v.second);
  }
}

template < class Key, class Value, class Hash, class Equal >
void dirko::HashTable< Key, Value, Hash, Equal >::add(Key k, Value v)
{
  size_t id = hasher_(k) % slots_;
  for (LIter< std::pair< Key, Value > > it = data_[id].begin(); it != data_[id].end(); ++it) {
    if (comparator_((*it).first, k)) {
      (*it).second = v;
      return;
    }
  }
  data_[id].push_back({k, v});
  ++elements_;
}

template < class Key, class Value, class Hash, class Equal >
void dirko::HashTable< Key, Value, Hash, Equal >::drop(Key k)
{
  size_t id = hasher_(k) % slots_;
  for (LIter< std::pair< Key, Value > > it = data_[id].begin(); it != data_[id].end(); ++it) {
    if (comparator_((*it).first, k)) {
      data_[id].erase(it);
      --elements_;
      return;
    }
  }
}
template < class Key, class Value, class Hash, class Equal >
Value dirko::HashTable< Key, Value, Hash, Equal >::get(Key k)
{
  size_t id = hasher_(k) % slots_;
  for (LIter< std::pair< Key, Value > > it = data_[id].begin(); it != data_[id].end(); ++it) {
    if (comparator_((*it).first, k)) {
      return (*it).second;
    }
  }
  throw std::out_of_range("No such element");
}
template < class Key, class Value, class Hash, class Equal >
const Value dirko::HashTable< Key, Value, Hash, Equal >::get(Key k) const
{
  return get(k);
}
template < class Key, class Value, class Hash, class Equal >
bool dirko::HashTable< Key, Value, Hash, Equal >::has(Key k) const noexcept
{

  size_t id = hasher_(k) % slots_;
  for (LCIter< std::pair< Key, Value > > it = data_[id].cbegin(); it != data_[id].cend(); ++it) {
    if (comparator_((*it).first, k)) {
      return true;
    }
  }
  return false;
}
template < class Key, class Value, class Hash, class Equal >
void dirko::HashTable< Key, Value, Hash, Equal >::rehash(size_t slots)
{
  HashTable< Key, Value, Hash, Equal > cpy(slots);
  for (const std::pair< Key, Value > &v : *this) {
    cpy.add(v.first, v.second);
  }
  swap(cpy);
}
template < class Key, class Value, class Hash, class Equal >

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
  std::swap(hasher_, other.hasher_);
  std::swap(comparator_, other.comparator_);
  std::swap(slots_, other.slots_);
  std::swap(elements_, other.elements_);
}

template < class Key, class Value, class Hash, class Equal >
dirko::HTIter< Key, Value, Hash, Equal > dirko::HashTable< Key, Value, Hash, Equal >::begin() noexcept
{
  return {this, data_[0].begin(), 0};
}
template < class Key, class Value, class Hash, class Equal >
dirko::HTIter< Key, Value, Hash, Equal > dirko::HashTable< Key, Value, Hash, Equal >::end() noexcept
{
  return {this, data_[slots_ - 1].end(), slots_ - 1};
}
template < class Key, class Value, class Hash, class Equal >
dirko::HTCIter< Key, Value, Hash, Equal > dirko::HashTable< Key, Value, Hash, Equal >::cbegin() const noexcept
{
  return {this, data_[0].cbegin(), 0};
}
template < class Key, class Value, class Hash, class Equal >
dirko::HTCIter< Key, Value, Hash, Equal > dirko::HashTable< Key, Value, Hash, Equal >::cend() const noexcept
{
  return {this, data_[slots_ - 1].cend(), slots_ - 1};
}
template < class Key, class Value, class Hash, class Equal >
dirko::HTIter< Key, Value, Hash, Equal >::HTIter(HashTable< Key, Value, Hash, Equal > *table,
                                                 LIter< std::pair< Key, Value > > curr, size_t slot):
  table_(table),
  curr_(curr),
  slot_(slot)
{}
template < class Key, class Value, class Hash, class Equal >
std::pair< Key, Value > &dirko::HTIter< Key, Value, Hash, Equal >::operator*()
{
  return *curr_;
}
template < class Key, class Value, class Hash, class Equal >
dirko::HTIter< Key, Value, Hash, Equal > &dirko::HTIter< Key, Value, Hash, Equal >::operator++()
{
  if (curr_ == table_->data_[slot_].end()) {
    if (!(slot_ < table_->size())) {
      throw std::out_of_range("out of bounds");
    }
    ++slot_;
    curr_ = table_->data_[slot_].begin();
  } else {
    ++curr_;
  }
  return *this;
}
template < class Key, class Value, class Hash, class Equal >
dirko::HTIter< Key, Value, Hash, Equal > &dirko::HTIter< Key, Value, Hash, Equal >::operator--()
{
  if (curr_.curr_->prev == nullptr) {
    if (slot_ < 1) {
      throw std::out_of_range("out of bounds");
    }
    --slot_;
    curr_ = table_->data_[slot_].end();
  }
  --curr_;
  return *this;
}
template < class Key, class Value, class Hash, class Equal >
dirko::HTIter< Key, Value, Hash, Equal > dirko::HTIter< Key, Value, Hash, Equal >::operator++(int)
{
  HTIter< Key, Value, Hash, Equal > past = curr_;
  if (curr_ == table_->data_[slot_].end()) {
    if (!(slot_ < table_->size())) {
      throw std::out_of_range("out of bounds");
    }
    ++slot_;
    curr_ = table_->data_[slot_].begin();
  } else {
    ++curr_;
  }
  return past;
}
template < class Key, class Value, class Hash, class Equal >
dirko::HTIter< Key, Value, Hash, Equal > dirko::HTIter< Key, Value, Hash, Equal >::operator--(int)
{
  HTIter< Key, Value, Hash, Equal > past = curr_;
  if (curr_.curr_->prev == nullptr) {
    if (slot_ < 1) {
      throw std::out_of_range("out of bounds");
    }
    --slot_;
    curr_ = table_->data_[slot_].end();
  }
  --curr_;
  return past;
}
template < class Key, class Value, class Hash, class Equal >
bool dirko::HTIter< Key, Value, Hash, Equal >::operator==(const HTIter< Key, Value, Hash, Equal > &other) const
{
  return ((*curr_).first == (*other.curr_).first) & ((*curr_).second == (*other.curr_).second);
}
template < class Key, class Value, class Hash, class Equal >
bool dirko::HTIter< Key, Value, Hash, Equal >::operator!=(const HTIter< Key, Value, Hash, Equal > &other) const
{
  return !(*this == other);
}

template < class Key, class Value, class Hash, class Equal >
dirko::HTCIter< Key, Value, Hash, Equal >::HTCIter(HashTable< Key, Value, Hash, Equal > *table,
                                                   LCIter< std::pair< Key, Value > > curr, size_t slot):
  table_(table),
  curr_(curr),
  slot_(slot)
{}
template < class Key, class Value, class Hash, class Equal >
const std::pair< Key, Value > &dirko::HTCIter< Key, Value, Hash, Equal >::operator*() const
{
  return *curr_;
}
template < class Key, class Value, class Hash, class Equal >
dirko::HTCIter< Key, Value, Hash, Equal > &dirko::HTCIter< Key, Value, Hash, Equal >::operator++()
{
  if (curr_.curr_->next == nullptr) {
    if (!(slot_ < table_->size())) {
      throw std::out_of_range("out of bounds");
    }
    ++slot_;
    curr_ = table_->data_[slot_].begin();
  } else {
    ++curr_;
  }
  return curr_;
}
template < class Key, class Value, class Hash, class Equal >
dirko::HTCIter< Key, Value, Hash, Equal > &dirko::HTCIter< Key, Value, Hash, Equal >::operator--()
{
  if (curr_.curr_->prev == nullptr) {
    if (slot_ < 1) {
      throw std::out_of_range("out of bounds");
    }
    --slot_;
    curr_ = table_->data_[slot_].end();
  }
  --curr_;
  return curr_;
}
template < class Key, class Value, class Hash, class Equal >
dirko::HTCIter< Key, Value, Hash, Equal > dirko::HTCIter< Key, Value, Hash, Equal >::operator++(int)
{
  HTCIter< Key, Value, Hash, Equal > past = curr_;
  if (curr_.curr_->next == nullptr) {
    if (!(slot_ < table_->size())) {
      throw std::out_of_range("out of bounds");
    }
    ++slot_;
    curr_ = table_->data_[slot_].begin();
  } else {
    ++curr_;
  }
  return past;
}
template < class Key, class Value, class Hash, class Equal >
dirko::HTCIter< Key, Value, Hash, Equal > dirko::HTCIter< Key, Value, Hash, Equal >::operator--(int)
{
  HTCIter< Key, Value, Hash, Equal > past = curr_;
  if (curr_.curr_->prev == nullptr) {
    if (slot_ < 1) {
      throw std::out_of_range("out of bounds");
    }
    --slot_;
    curr_ = table_->data_[slot_].end();
  }
  --curr_;
  return past;
}
template < class Key, class Value, class Hash, class Equal >
bool dirko::HTCIter< Key, Value, Hash, Equal >::operator==(const HTCIter< Key, Value, Hash, Equal > &other) const
{
  return ((*curr_).first == (*other.curr_).first) & ((*curr_).second == (*other.curr_).second);
}
template < class Key, class Value, class Hash, class Equal >
bool dirko::HTCIter< Key, Value, Hash, Equal >::operator!=(const HTCIter< Key, Value, Hash, Equal > &other) const
{
  return !(*this == other);
}

template < class Key, class Value, class Hash, class Equal >
dirko::HashTable< Key, Value, Hash, Equal >::HashTable(const HashTable &other):
  HashTable(other.slots_)
{
  for (size_t i = 0; i < slots_; ++i) {
    for (LCIter< std::pair< Key, Value > > it = other.data_[i].cbegin(); it != other.data_[i].cend(); ++it) {
      data_[i].push_back(*it);
      ++elements_;
    }
  }
}
template < class Key, class Value, class Hash, class Equal >
dirko::HashTable< Key, Value, Hash, Equal >::HashTable(HashTable &&other) noexcept:
  data_(std::move(other.data_)),
  hasher_(std::move(other.hasher_)),
  comparator_(std::move(other.comparator_)),
  slots_(other.slots_),
  elements_(other.elements_)
{
  other.slots_ = 0;
  other.elements_ = 0;
}
template < class Key, class Value, class Hash, class Equal >
dirko::HashTable< Key, Value, Hash, Equal > &
dirko::HashTable< Key, Value, Hash, Equal >::operator=(const HashTable &other)
{
  if (this == std::addressof(other)) {
    return *this;
  }
  HashTable< Key, Value, Hash, Equal > cpy(other);
  swap(cpy);
  return *this;
}
template < class Key, class Value, class Hash, class Equal >
dirko::HashTable< Key, Value, Hash, Equal > &
dirko::HashTable< Key, Value, Hash, Equal >::operator=(HashTable &&other) noexcept
{
  if (this == std::addressof(other)) {
    return *this;
  }
  swap(other);
  return *this;
}

template < class Key, class Value, class Hash, class Equal >
dirko::HashTable< Key, Value, Hash, Equal >::~HashTable()
{
  clear();
}
#endif
