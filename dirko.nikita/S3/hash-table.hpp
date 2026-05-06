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

    void add(Key k, Value v);
    void drop(Key k);
    Value &get(Key k);
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
    HTIter();
    HTIter(Vector< List< std::pair< Key, Value > > > *, size_t);

    HTIter &operator++();
    HTIter operator++(int);
    bool operator==(const HTIter &other) const noexcept;
    bool operator!=(const HTIter &other) const noexcept;
    std::pair< Key, Value > &operator*() noexcept;

  private:
    Vector< List< std::pair< Key, Value > > > *data_;
    size_t slot_;
    LIter< std::pair< Key, Value > > lit_, lend_;
    void next();
  };
  template < class Key, class Value, class Hash, class Equal >
  class HTCIter
  {
  public:
    HTCIter();
    HTCIter(Vector< List< std::pair< Key, Value > > > *, size_t);

    HTCIter &operator++();
    HTCIter operator++(int);
    bool operator==(const HTCIter &other) const noexcept;
    bool operator!=(const HTCIter &other) const noexcept;
    std::pair< Key, Value > &operator*() noexcept;

  private:
    Vector< List< std::pair< Key, Value > > > *data_;
    size_t slot_;
    LCIter< std::pair< Key, Value > > lit_, lend_;
    void next();
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
  data_.reserve(slots_);
  for (size_t i = 0; i < slots_; ++i) {
    data_.pushBack(List< std::pair< Key, Value > >());
  }
}

template < class Key, class Value, class Hash, class Equal >
dirko::HashTable< Key, Value, Hash, Equal >::HashTable(std::initializer_list< std::pair< Key, Value > > il):
  HashTable(il.size())
{
  data_.reserve(il.size());
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
Value &dirko::HashTable< Key, Value, Hash, Equal >::get(Key k)
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
  return {&data_, 0};
}
template < class Key, class Value, class Hash, class Equal >
dirko::HTIter< Key, Value, Hash, Equal > dirko::HashTable< Key, Value, Hash, Equal >::end() noexcept
{
  return HTIter< Key, Value, Hash, Equal >();
}
template < class Key, class Value, class Hash, class Equal >
dirko::HTCIter< Key, Value, Hash, Equal > dirko::HashTable< Key, Value, Hash, Equal >::cbegin() const noexcept
{
  return {&data_, 0};
}
template < class Key, class Value, class Hash, class Equal >
dirko::HTCIter< Key, Value, Hash, Equal > dirko::HashTable< Key, Value, Hash, Equal >::cend() const noexcept
{
  return HTCIter< Key, Value, Hash, Equal >();
}

template < class Key, class Value, class Hash, class Equal >
void dirko::HTIter< Key, Value, Hash, Equal >::next()
{
  while (slot_ < data_->getSize()) {
    lit_ = (*data_)[slot_].begin();
    lend_ = (*data_)[slot_].end();
    if (lit_ != lend_) {
      return;
    }
    ++slot_;
  }
  data_ = nullptr;
}

template < class Key, class Value, class Hash, class Equal >
dirko::HTIter< Key, Value, Hash, Equal >::HTIter(Vector< List< std::pair< Key, Value > > > *data, size_t slot):
  data_(data),
  slot_(slot),
  lit_(nullptr),
  lend_(nullptr)
{
  next();
}
template < class Key, class Value, class Hash, class Equal >
dirko::HTIter< Key, Value, Hash, Equal > &dirko::HTIter< Key, Value, Hash, Equal >::operator++()
{
  ++lit_;
  if (lit_ != lend_) {
    return *this;
  }
  ++slot_;
  next();
  return *this;
}
template < class Key, class Value, class Hash, class Equal >
dirko::HTIter< Key, Value, Hash, Equal > dirko::HTIter< Key, Value, Hash, Equal >::operator++(int)
{
  HTIter< Key, Value, Hash, Equal > prev = *this;
  ++lit_;
  if (lit_ != lend_) {
    return prev;
  }
  ++slot_;
  next();
  return prev;
}
template < class Key, class Value, class Hash, class Equal >
bool dirko::HTIter< Key, Value, Hash, Equal >::operator==(const HTIter &other) const noexcept
{
  if (data_ == nullptr && other.data_ == nullptr) {
    return true;
  }
  if (data_ == nullptr || other.data_ == nullptr) {
    return false;
  }
  if (slot_ >= data_->getSize() && other.slot_ >= other.data_->getSize()) {
    return true;
  }
  return lit_ == other.lit_ && slot_ == other.slot_;
}
template < class Key, class Value, class Hash, class Equal >
bool dirko::HTIter< Key, Value, Hash, Equal >::operator!=(const HTIter &other) const noexcept
{
  return !(*this == other);
}
template < class Key, class Value, class Hash, class Equal >
std::pair< Key, Value > &dirko::HTIter< Key, Value, Hash, Equal >::operator*() noexcept
{
  return *lit_;
}

template < class Key, class Value, class Hash, class Equal >
dirko::HTIter< Key, Value, Hash, Equal >::HTIter():
  data_(nullptr),
  slot_(0),
  lit_(nullptr),
  lend_(nullptr)
{}

template < class Key, class Value, class Hash, class Equal >
dirko::HTCIter< Key, Value, Hash, Equal >::HTCIter():
  data_(nullptr),
  slot_(0),
  lit_(nullptr),
  lend_(nullptr)
{}
template < class Key, class Value, class Hash, class Equal >
void dirko::HTCIter< Key, Value, Hash, Equal >::next()
{
  while (slot_ < data_->getSize()) {
    lit_ = (*data_)[slot_].cbegin();
    lend_ = (*data_)[slot_].cend();
    if (lit_ != lend_) {
      return;
    }
    ++slot_;
  }
  data_ = nullptr;
}

template < class Key, class Value, class Hash, class Equal >
dirko::HTCIter< Key, Value, Hash, Equal >::HTCIter(Vector< List< std::pair< Key, Value > > > *data, size_t slot):
  data_(data),
  slot_(slot),
  lit_(nullptr),
  lend_(nullptr)
{
  next();
}
template < class Key, class Value, class Hash, class Equal >
dirko::HTCIter< Key, Value, Hash, Equal > &dirko::HTCIter< Key, Value, Hash, Equal >::operator++()
{
  ++lit_;
  if (lit_ != lend_) {
    return *this;
  }
  ++slot_;
  next();
  return *this;
}
template < class Key, class Value, class Hash, class Equal >
dirko::HTCIter< Key, Value, Hash, Equal > dirko::HTCIter< Key, Value, Hash, Equal >::operator++(int)
{
  HTCIter< Key, Value, Hash, Equal > prev = *this;
  ++lit_;
  if (lit_ != lend_) {
    return prev;
  }
  ++slot_;
  next();
  return prev;
}
template < class Key, class Value, class Hash, class Equal >
bool dirko::HTCIter< Key, Value, Hash, Equal >::operator==(const HTCIter &other) const noexcept
{
  if (data_ == nullptr && other.data_ == nullptr) {
    return true;
  }
  if (data_ == nullptr || other.data_ == nullptr) {
    return false;
  }
  if (slot_ >= data_->getSize() && other.slot_ >= other.data_->getSize()) {
    return true;
  }
  return lit_ == other.lit_ && slot_ == other.slot_;
}
template < class Key, class Value, class Hash, class Equal >
bool dirko::HTCIter< Key, Value, Hash, Equal >::operator!=(const HTCIter &other) const noexcept
{
  return !(*this == other);
}
template < class Key, class Value, class Hash, class Equal >
std::pair< Key, Value > &dirko::HTCIter< Key, Value, Hash, Equal >::operator*() noexcept
{
  return *lit_;
}
#endif
