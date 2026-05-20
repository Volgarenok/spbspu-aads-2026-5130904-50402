#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>
#include <list.hpp>
#include <top-it-vector.hpp>
#include "HashFunction.hpp"
#include "HashIter.hpp"

namespace afanasev
{
  template< class Key, class Value, class Hash, class Equal >
  class HashTable
  {
    friend class HashIter< Key, Value, Hash, Equal >;
    friend class HashConstIter< Key, Value, Hash, Equal >;

    using type = std::pair< Key, Value >;

    public:
    using HIter = HashIter< Key, Value, Hash, Equal >;
    using HCIter = HashConstIter< Key, Value, Hash, Equal >;

    HIter begin();
    HIter end();
    HCIter cbegin() const;
    HCIter cend() const;

    explicit HashTable(size_t slots);
    HashTable(const HashTable & other);
    HashTable(HashTable && other) noexcept;
    HashTable & operator=(const HashTable & other);
    HashTable & operator=(HashTable && other) noexcept;
    ~HashTable();

    void clear() noexcept;


    void add(Key k, Value v);
    Value drop(Key k);
    bool has(Key k) const noexcept;
    void rehash(size_t slots);
    void swap(HashTable & other) noexcept;

    Value & get(Key k);
    const Value & get(Key k) const;

    size_t size() const noexcept;
    bool empty() const noexcept;

    private:
    Vector< List< type > > data_;
    size_t capacity_;
    size_t size_;
    Hash hasher_;
    Equal comparator_;
  };
}

template < class Key, class Value, class Hash, class Equal >
typename afanasev::HashTable< Key, Value, Hash, Equal >::HIter
afanasev::HashTable< Key, Value, Hash, Equal >::begin()
{
  return HIter(& data_, capacity_, 0);
}

template < class Key, class Value, class Hash, class Equal >
typename afanasev::HashTable< Key, Value, Hash, Equal >::HIter
afanasev::HashTable< Key, Value, Hash, Equal >::end()
{
  return HIter();
}

template < class Key, class Value, class Hash, class Equal >
typename afanasev::HashTable< Key, Value, Hash, Equal >::HCIter
afanasev::HashTable< Key, Value, Hash, Equal >::cbegin() const
{
  return HCIter(& data_, capacity_, 0);
}

template < class Key, class Value, class Hash, class Equal >
typename afanasev::HashTable< Key, Value, Hash, Equal >::HCIter
afanasev::HashTable< Key, Value, Hash, Equal >::cend() const
{
  return HCIter();
}

template < class Key, class Value, class Hash, class Equal >
Value & afanasev::HashTable< Key, Value, Hash, Equal >::get(Key k)
{
  size_t idx = hasher_(k) % capacity_;
  List< type > & bucket = data_[idx];
  LIter< type > it = bucket.begin();
  while (it != LIter< type >())
  {
    if (comparator_((*it).first, k))
    {
      return (*it).second;
    }
    ++it;
  }
  throw std::out_of_range("Key not found");
}

template < class Key, class Value, class Hash, class Equal >
const Value & afanasev::HashTable< Key, Value, Hash, Equal >::get(Key k) const
{
  size_t idx = hasher_(k) % capacity_;
  const List< type > & bucket = data_[idx];
  LCIter< type > it = bucket.begin();
  while (it != LCIter< type >())
  {
    if (comparator_((*it).first, k))
    {
      return (*it).second;
    }
    ++it;
  }
  throw std::out_of_range("Key not found");
}

template < class Key, class Value, class Hash, class Equal >
afanasev::HashTable< Key, Value, Hash, Equal >::HashTable(const HashTable & other):
  data_(),
  capacity_(other.capacity_),
  size_(0),
  hasher_(other.hasher_),
  comparator_(other.comparator_)
{
  for (size_t i = 0; i < capacity_; ++i)
  {
    data_.pushBack(List< type >());
  }

  for (size_t i = 0; i < capacity_; ++i)
  {
    const List< type > & srcBucket = other.data_[i];
    LCIter< type > it = srcBucket.begin();
    while (it != LCIter< type >())
    {
      data_[i].pushFront(*it);
      ++size_;
      ++it;
    }
  }
}

template < class Key, class Value, class Hash, class Equal >
afanasev::HashTable< Key, Value, Hash, Equal >::HashTable(HashTable && other) noexcept:
  data_(std::move(other.data_)),
  capacity_(other.capacity_),
  size_(other.size_),
  hasher_(std::move(other.hasher_)),
  comparator_(std::move(other.comparator_))
{
  other.capacity_ = 0;
  other.size_ = 0;
  other.data_ = Vector< List < type > >();
}

template <class Key, class Value, class Hash, class Equal>
afanasev::HashTable<Key, Value, Hash, Equal> &
afanasev::HashTable<Key, Value, Hash, Equal>::operator=(const HashTable & other)
{
  if (this != & other)
  {
    HashTable tmp(other);
    swap(tmp);
  }
  return *this;
}

template < class Key, class Value, class Hash, class Equal >
afanasev::HashTable< Key, Value, Hash, Equal > &
afanasev::HashTable< Key, Value, Hash, Equal >::operator=(HashTable && other) noexcept
{
  if (this != & other)
  {
    HashTable tmp(std::move(other));
    swap(tmp);
  }
  return *this;
}

template < class Key, class Value, class Hash, class Equal >
void afanasev::HashTable< Key, Value, Hash, Equal >::swap(HashTable & other) noexcept
{
  data_.swap(other.data_);
  std::swap(capacity_, other.capacity_);
  std::swap(size_, other.size_);
  std::swap(hasher_, other.hasher_);
  std::swap(comparator_, other.comparator_);
}

template < class Key, class Value, class Hash, class Equal >
size_t afanasev::HashTable< Key, Value, Hash, Equal >::size() const noexcept
{
  return size_;
}

template < class Key, class Value, class Hash, class Equal >
bool afanasev::HashTable< Key, Value, Hash, Equal >::empty() const noexcept
{
  return !size_;
}

template < class Key, class Value, class Hash, class Equal >
void afanasev::HashTable< Key, Value, Hash, Equal >::rehash(size_t slots)
{
  if (slots <= capacity_)
  {
    return;
  }

  Vector< List < type > > new_data;

  for (size_t i = 0; i < slots; ++i)
  {
    new_data.pushBack(List< type >());
  }

  for (size_t i = 0; i < capacity_; ++i)
  {
    List< type > & bucket = data_[i];
    LIter< type > it = bucket.begin();

    while (it != LIter< type >())
    {
      size_t new_idx = hasher_((*it).first) % slots;
      new_data[new_idx].pushFront(*it);
      ++it;
    }
  }

  data_ = std::move(new_data);
  capacity_ = slots;
}

template < class Key, class Value, class Hash, class Equal >
bool afanasev::HashTable< Key, Value, Hash, Equal >::has(Key k) const noexcept
{
  size_t idx = hasher_(k) % capacity_;
  const List< type > & bucket = data_[idx];
  LCIter< type > it = bucket.begin();

  while (it != LCIter< type >())
  {
    if (comparator_((*it).first, k)) return true;
    ++it;
  }

  return false;
}

template < class Key, class Value, class Hash, class Equal >
Value afanasev::HashTable< Key, Value, Hash, Equal >::drop(Key k)
{
  size_t idx = hasher_(k) % capacity_;
  List< type > & bucket = data_[idx];
  LIter< type > prev = LIter< type >();
  LIter< type > it = bucket.begin();

  while (it != LIter< type >())
  {
    if (comparator_((*it).first, k))
    {
      Value val = (*it).second;
      if (prev == LIter< type >())
      {
        bucket.popFront();
      }
      else
      {
        bucket.deleteNext(prev);
      }
      --size_;
      return val;
    }
    prev = it;
    ++it;
  }
  throw std::out_of_range("Key not found");
}

template < class Key, class Value, class Hash, class Equal >
void afanasev::HashTable< Key, Value, Hash, Equal >::add(Key k, Value v)
{
  size_t idx = hasher_(k) % capacity_;
  List< type > & bucket = data_[idx];
  LIter< type > it = bucket.begin();

  while (it != LIter< type >())
  {
    if (comparator_((*it).first, k))
    {
      (*it).second = v;
      return;
    }
    ++it;
  }

  bucket.pushFront(type(k, v));
  ++size_;
}

template < class Key, class Value, class Hash, class Equal >
void afanasev::HashTable< Key, Value, Hash, Equal >::clear() noexcept
{
  for (size_t i = 0; i < capacity_; ++i)
  {
    data_[i].clear();
  }

  size_ = 0;
}

template < class Key, class Value, class Hash, class Equal >
afanasev::HashTable< Key, Value, Hash, Equal >::HashTable(size_t slots):
  data_(),
  capacity_(slots),
  size_(0),
  hasher_(Hash{}),
  comparator_(Equal{})
{
  for (size_t i = 0; i < slots; ++i)
  {
    data_.pushBack(List< type >());
  }
}

template < class Key, class Value, class Hash, class Equal >
afanasev::HashTable< Key, Value, Hash, Equal >::~HashTable()
{
  clear();
}

#endif
