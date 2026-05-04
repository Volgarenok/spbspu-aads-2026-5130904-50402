#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP
#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>
#include "../common/Vector.hpp"
#include "../common/list.hpp"
#include "hashIters.hpp"

namespace karpovich
{

  template < class Key, class Value, class Hash = std::hash< Value >, class Equal = std::equal_to< Key > >
  class HashTable
  {
    friend class HashIter< Key, Value, Hash, Equal >;
    friend class HashConstIter< Key, Value, Hash, Equal >;
    using valType = std::pair< Key, Value >;

  public:
    using HIter = HashIter< Key, Value, Hash, Equal >;
    using HCIter = HashConstIter< Key, Value, Hash, Equal >;

    explicit HashTable(size_t slots);
    ~HashTable();

    HashTable(const HashTable &other);
    HashTable(HashTable &&other) noexcept;
    HashTable &operator=(const HashTable &other);
    HashTable &operator=(HashTable &&other) noexcept;

    void add(Key k, Value v);
    Value drop(Key k);
    Value &get(Key k);
    const Value &get(Key k) const;
    bool has(Key k) const noexcept;
    void rehash(size_t slots);

    void clear() noexcept;
    size_t size() const noexcept;
    bool empty() const noexcept;
    void swap(HashTable &other) noexcept;

    HIter begin();
    HIter end();
    HCIter cbegin() const;
    HCIter cend() const;

  private:
    Vector< List< valType > > data_;
    size_t capacity_;
    size_t size_;
    Hash hasher_;
    Equal comparator_;
  };

}

template < class Key, class Value, class Hash, class Equal >
karpovich::HashTable< Key, Value, Hash, Equal >::HashTable(size_t slots):
  data_(),
  capacity_(slots),
  size_(0),
  hasher_(Hash{}),
  comparator_(Equal{})
{
  for (size_t i = 0; i < slots; ++i) {
    data_.pushBack(List< valType >());
  }
}

template < class Key, class Value, class Hash, class Equal >
karpovich::HashTable< Key, Value, Hash, Equal >::~HashTable()
{
  clear();
}

template < class Key, class Value, class Hash, class Equal >
karpovich::HashTable< Key, Value, Hash, Equal >::HashTable(HashTable &&other) noexcept:
  data_(std::move(other.data_)),
  capacity_(other.capacity_),
  size_(other.size_),
  hasher_(std::move(other.hasher_)),
  comparator_(std::move(other.comparator_))
{
  other.capacity_ = 0;
  other.size_ = 0;
}

template < class Key, class Value, class Hash, class Equal >
karpovich::HashTable< Key, Value, Hash, Equal >::HashTable(const HashTable &other):
  data_(),
  capacity_(other.capacity_),
  size_(0),
  hasher_(other.hasher_),
  comparator_(other.comparator_)
{
  for (size_t i = 0; i < capacity_; ++i) {
    data_.pushBack(List< valType >());
  }
  for (size_t i = 0; i < other.capacity_; ++i) {
    auto &src = const_cast< List< valType > & >(other.data_[i]);
    for (auto it = src.begin(); it != src.end(); ++it) {
      data_[i].push_back(*it);
      ++size_;
    }
  }
}

template < class Key, class Value, class Hash, class Equal >
karpovich::HashTable< Key, Value, Hash, Equal > &
karpovich::HashTable< Key, Value, Hash, Equal >::operator=(const HashTable &other)
{
  if (this == std::addressof(other)) {
    return *this;
  }
  HashTable< Key, Value, Hash, Equal > cpy(other);
  swap(cpy);
  return *this;
}

template < class Key, class Value, class Hash, class Equal >
karpovich::HashTable< Key, Value, Hash, Equal > &
karpovich::HashTable< Key, Value, Hash, Equal >::operator=(HashTable &&other) noexcept
{
  if (this == std::addressof(other)) {
    return *this;
  }
  swap(other);
  return *this;
}

template < class Key, class Value, class Hash, class Equal >
void karpovich::HashTable< Key, Value, Hash, Equal >::add(Key k, Value v)
{
  size_t idx = hasher_(k) % capacity_;
  for (auto it = data_[idx].begin(); it != data_[idx].end(); ++it) {
    if (comparator_((*it).first, k)) {
      (*it).second = v;
      return;
    }
  }
  data_[idx].push_back(valType(k, v));
  ++size_;
}

template < class Key, class Value, class Hash, class Equal >
Value karpovich::HashTable< Key, Value, Hash, Equal >::drop(Key k)
{
  size_t idx = hasher_(k) % capacity_;
  for (auto it = data_[idx].begin(); it != data_[idx].end(); ++it) {
    if (comparator_((*it).first, k)) {
      Value val = (*it).second;
      data_[idx].erase(it);
      --size_;
      return val;
    }
  }
  throw std::out_of_range("Key not found");
}

template < class Key, class Value, class Hash, class Equal >
Value &karpovich::HashTable< Key, Value, Hash, Equal >::get(Key k)
{
  size_t idx = hasher_(k) % capacity_;
  for (auto it = data_[idx].begin(); it != data_[idx].end(); ++it) {
    if (comparator_((*it).first, k)) {
      return (*it).second;
    }
  }
  throw std::out_of_range("Key not found");
}

template < class Key, class Value, class Hash, class Equal >
const Value &karpovich::HashTable< Key, Value, Hash, Equal >::get(Key k) const
{
  size_t idx = hasher_(k) % capacity_;
  auto &bucket = const_cast< List< valType > & >(data_[idx]);
  for (auto it = bucket.begin(); it != bucket.end(); ++it) {
    if (comparator_((*it).first, k)) {
      return (*it).second;
    }
  }
  throw std::out_of_range("Key not found");
}

template < class Key, class Value, class Hash, class Equal >
bool karpovich::HashTable< Key, Value, Hash, Equal >::has(Key k) const noexcept
{
  size_t idx = hasher_(k) % capacity_;
  auto &bucket = const_cast< List< valType > & >(data_[idx]);
  for (auto it = bucket.begin(); it != bucket.end(); ++it) {
    if (comparator_((*it).first, k)) {
      return true;
    }
  }
  return false;
}

template < class Key, class Value, class Hash, class Equal >
void karpovich::HashTable< Key, Value, Hash, Equal >::rehash(size_t slots)
{
  if (slots <= capacity_) {
    return;
  }
  Vector< List< valType > > new_data;
  for (size_t i = 0; i < slots; ++i) {
    new_data.pushBack(List< valType >());
  }
  for (size_t i = 0; i < capacity_; ++i) {
    for (auto it = data_[i].begin(); it != data_[i].end(); ++it) {
      size_t idx = hasher_((*it).first) % slots;
      new_data[idx].pushBack(*it);
    }
  }
  data_ = std::move(new_data);
  capacity_ = slots;
}

template < class Key, class Value, class Hash, class Equal >
void karpovich::HashTable< Key, Value, Hash, Equal >::clear() noexcept
{
  for (size_t i = 0; i < capacity_; ++i) {
    data_[i].clear();
  }
  size_ = 0;
}

template < class Key, class Value, class Hash, class Equal >
size_t karpovich::HashTable< Key, Value, Hash, Equal >::size() const noexcept
{
  return size_;
}

template < class Key, class Value, class Hash, class Equal >
bool karpovich::HashTable< Key, Value, Hash, Equal >::empty() const noexcept
{
  return size_ == 0;
}

template < class Key, class Value, class Hash, class Equal >
void karpovich::HashTable< Key, Value, Hash, Equal >::swap(HashTable &other) noexcept
{
  data_.swap(other.data_);
  std::swap(hasher_, other.hasher_);
  std::swap(comparator_, other.comparator_);
  std::swap(capacity_, other.capacity_);
  std::swap(size_, other.size_);
}

template < class Key, class Value, class Hash, class Equal >
karpovich::HashIter< Key, Value, Hash, Equal > karpovich::HashTable< Key, Value, Hash, Equal >::begin()
{
  return HIter(&data_, capacity_, 0);
}

template < class Key, class Value, class Hash, class Equal >
karpovich::HashIter< Key, Value, Hash, Equal > karpovich::HashTable< Key, Value, Hash, Equal >::end()
{
  return HIter();
}

template < class Key, class Value, class Hash, class Equal >
karpovich::HashConstIter< Key, Value, Hash, Equal > karpovich::HashTable< Key, Value, Hash, Equal >::cbegin() const
{
  return HCIter(&data_, capacity_, 0);
}

template < class Key, class Value, class Hash, class Equal >
karpovich::HashConstIter< Key, Value, Hash, Equal > karpovich::HashTable< Key, Value, Hash, Equal >::cend() const
{
  return HCIter();
}

#endif
