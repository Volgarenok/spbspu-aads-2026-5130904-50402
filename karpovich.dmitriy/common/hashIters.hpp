#ifndef HASH_ITERS_HPP
#define HASH_ITERS_HPP

#include <cstddef>
#include <utility>
#include "Vector.hpp"
#include "list.hpp"

namespace karpovich
{
  template< class Key, class Value, class Hash, class Equal >
  class HashTable;

  template< class Key, class Value, class Hash, class Equal >
  class HashConstIter;

  template< class Key, class Value, class Hash, class Equal >
  class HashIter
  {
    friend class HashTable< Key, Value, Hash, Equal >;
    friend class HashConstIter< Key, Value, Hash, Equal >;
    using valType = std::pair< Key, Value >;
    using list_t = List< valType >;

  public:
    HashIter();
    HashIter(Vector< list_t > *data, size_t capacity, size_t start_idx);

    HashIter &operator++();
    bool operator==(const HashIter &other) const;
    bool operator!=(const HashIter &other) const;
    valType &operator*() const;

  private:
    Vector< list_t > *data_;
    size_t capacity_;
    size_t idx_;
    LIter< valType > listIt_;
    LIter< valType > listEnd_;

    void findValid();
  };

  template< class Key, class Value, class Hash, class Equal >
  class HashConstIter
  {
    friend class HashTable< Key, Value, Hash, Equal >;
    friend class HashIter< Key, Value, Hash, Equal >;
    using valType = std::pair< Key, Value >;
    using list_t = List< valType >;

  public:
    HashConstIter();
    HashConstIter(const Vector< list_t > *data, size_t capacity, size_t start_idx);

    HashConstIter &operator++();
    bool operator==(const HashConstIter &other) const;
    bool operator!=(const HashConstIter &other) const;
    const valType &operator*() const;

  private:
    const Vector< list_t > *data_;
    size_t capacity_;
    size_t idx_;
    LCIter< valType > listIt_;
    LCIter< valType > listEnd_;

    void findValid();
  };
}

template< class Key, class Value, class Hash, class Equal >
karpovich::HashIter< Key, Value, Hash, Equal >::HashIter():
  data_(nullptr),
  capacity_(0),
  idx_(0),
  listIt_(nullptr),
  listEnd_(nullptr)
{}

template< class Key, class Value, class Hash, class Equal >
karpovich::HashIter< Key, Value, Hash, Equal >::HashIter(Vector< list_t > *data, size_t cap, size_t idx):
  data_(data),
  capacity_(cap),
  idx_(idx),
  listIt_(nullptr),
  listEnd_(nullptr)
{
  findValid();
}

template< class Key, class Value, class Hash, class Equal >
void karpovich::HashIter< Key, Value, Hash, Equal >::findValid()
{
  while (idx_ < capacity_) {
    listIt_ = (*data_)[idx_].begin();
    listEnd_ = (*data_)[idx_].end();
    if (listIt_ != listEnd_) {
      return;
    }
    ++idx_;
  }
  data_ = nullptr;
}

template< class Key, class Value, class Hash, class Equal >
karpovich::HashIter< Key, Value, Hash, Equal > &karpovich::HashIter< Key, Value, Hash, Equal >::operator++()
{
  if (listIt_ != listEnd_) {
    ++listIt_;
    if (listIt_ != listEnd_) {
      return *this;
    }
  }
  ++idx_;
  findValid();
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
bool karpovich::HashIter< Key, Value, Hash, Equal >::operator==(const HashIter &other) const
{
  bool this_end = (data_ == nullptr || idx_ >= capacity_);
  bool other_end = (other.data_ == nullptr || other.idx_ >= other.capacity_);

  if (this_end && other_end) {
    return true;
  }
  if (this_end || other_end) {
    return false;
  }
  return idx_ == other.idx_ && listIt_ == other.listIt_;
}

template< class Key, class Value, class Hash, class Equal >
bool karpovich::HashIter< Key, Value, Hash, Equal >::operator!=(const HashIter &other) const
{
  return !(*this == other);
}

template< class Key, class Value, class Hash, class Equal >
typename karpovich::HashIter< Key, Value, Hash, Equal >::valType &
karpovich::HashIter< Key, Value, Hash, Equal >::operator*() const
{
  return *listIt_;
}

template< class Key, class Value, class Hash, class Equal >
karpovich::HashConstIter< Key, Value, Hash, Equal >::HashConstIter():
  data_(nullptr),
  capacity_(0),
  idx_(0),
  listIt_(nullptr),
  listEnd_(nullptr)
{}

template< class Key, class Value, class Hash, class Equal >
karpovich::HashConstIter< Key, Value, Hash, Equal >::HashConstIter(const Vector< list_t > *data, size_t cap,
                                                                   size_t idx):
  data_(data),
  capacity_(cap),
  idx_(idx),
  listIt_(nullptr),
  listEnd_(nullptr)
{
  findValid();
}

template< class Key, class Value, class Hash, class Equal >
void karpovich::HashConstIter< Key, Value, Hash, Equal >::findValid()
{
  while (idx_ < capacity_) {
    listIt_ = (*data_)[idx_].begin();
    listEnd_ = (*data_)[idx_].end();
    if (listIt_ != listEnd_) {
      return;
    }
    ++idx_;
  }
  data_ = nullptr;
}

template< class Key, class Value, class Hash, class Equal >
karpovich::HashConstIter< Key, Value, Hash, Equal > &karpovich::HashConstIter< Key, Value, Hash, Equal >::operator++()
{
  if (listIt_ != listEnd_) {
    ++listIt_;
    if (listIt_ != listEnd_) {
      return *this;
    }
  }
  ++idx_;
  findValid();
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
bool karpovich::HashConstIter< Key, Value, Hash, Equal >::operator==(const HashConstIter &other) const
{
  bool this_end = (data_ == nullptr || idx_ >= capacity_);
  bool other_end = (other.data_ == nullptr || other.idx_ >= other.capacity_);

  if (this_end && other_end) {
    return true;
  }
  if (this_end || other_end) {
    return false;
  }
  return idx_ == other.idx_ && listIt_ == other.listIt_;
}

template< class Key, class Value, class Hash, class Equal >
bool karpovich::HashConstIter< Key, Value, Hash, Equal >::operator!=(const HashConstIter &other) const
{
  return !(*this == other);
}

template< class Key, class Value, class Hash, class Equal >
const typename karpovich::HashConstIter< Key, Value, Hash, Equal >::valType &
karpovich::HashConstIter< Key, Value, Hash, Equal >::operator*() const
{
  return *listIt_;
}

#endif
