#ifndef HASH_ITERS_HPP
#define HASH_ITERS_HPP
#include <cstddef>
#include <utility>
#include "../common/Vector.hpp"
#include "../common/list.hpp"

namespace karpovich
{
  template < class Key, class Value, class Hash, class Equal >
  class HashTable;

  template < class Key, class Value, class Hash, class Equal >
  class HashConstIter;

  template < class Key, class Value, class Hash, class Equal >
  class HashIter
  {
    friend class HashTable< Key, Value, Hash, Equal >;
    friend class HashConstIter< Key, Value, Hash, Equal >;
    using valType = std::pair< Key, Value >;

  public:
    HashIter();
    HashIter(Vector< List< valType > > *data, size_t capacity, size_t start_idx);

    HashIter &operator++();
    bool operator==(const HashIter &other) const;
    bool operator!=(const HashIter &other) const;
    valType &operator*() const;

  private:
    Vector< List< valType > > *data_;
    size_t capacity_;
    size_t idx_;
    LIter< valType > listIt_;
    LIter< valType > listEnd_;

    void findValid();
  };

  template < class Key, class Value, class Hash, class Equal >
  class HashConstIter
  {
    friend class HashTable< Key, Value, Hash, Equal >;
    friend class HashIter< Key, Value, Hash, Equal >;
    using valType = std::pair< Key, Value >;

  public:
    HashConstIter();
    HashConstIter(const Vector< List< valType > > *data, size_t capacity, size_t start_idx);

    HashConstIter &operator++();
    bool operator==(const HashConstIter &other) const;
    bool operator!=(const HashConstIter &other) const;
    const valType &operator*() const;

  private:
    const Vector< List< valType > > *data_;
    size_t capacity_;
    size_t idx_;
    LCIter< valType > listIt_;
    LCIter< valType > listEnd_;

    void findValid();
  };
}

template < class Key, class Value, class Hash, class Equal >
karpovich::HashIter< Key, Value, Hash, Equal >::HashIter():
  data_(nullptr),
  capacity_(0),
  idx_(0),
  listIt_(nullptr),
  listEnd_(nullptr)
{}

template < class Key, class Value, class Hash, class Equal >
karpovich::HashIter< Key, Value, Hash, Equal >::HashIter(Vector< List< valType > > *data, size_t capacity,
                                                         size_t start_idx):
  data_(data),
  capacity_(capacity),
  idx_(start_idx),
  listIt_(nullptr),
  listEnd_(nullptr)
{
  findValid();
}

template < class Key, class Value, class Hash, class Equal >
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
}

template < class Key, class Value, class Hash, class Equal >
karpovich::HashIter< Key, Value, Hash, Equal > &karpovich::HashIter< Key, Value, Hash, Equal >::operator++()
{
  ++listIt_;
  findValid();
  return *this;
}

template < class Key, class Value, class Hash, class Equal >
bool karpovich::HashIter< Key, Value, Hash, Equal >::operator==(const HashIter &other) const
{
  if (data_ == nullptr && other.data_ == nullptr) {
    return true;
  }
  if (data_ == nullptr || other.data_ == nullptr) {
    return false;
  }
  if (idx_ >= capacity_ && other.idx_ >= other.capacity_) {
    return true;
  }
  return idx_ == other.idx_ && listIt_ == other.listIt_;
}

template < class Key, class Value, class Hash, class Equal >
bool karpovich::HashIter< Key, Value, Hash, Equal >::operator!=(const HashIter &other) const
{
  return !(*this == other);
}

template < class Key, class Value, class Hash, class Equal >
typename karpovich::HashIter< Key, Value, Hash, Equal >::valType &
karpovich::HashIter< Key, Value, Hash, Equal >::operator*() const
{
  return *listIt_;
}

template < class Key, class Value, class Hash, class Equal >
karpovich::HashConstIter< Key, Value, Hash, Equal >::HashConstIter():
  data_(nullptr),
  capacity_(0),
  idx_(0),
  listIt_(nullptr),
  listEnd_(nullptr)
{}

template < class Key, class Value, class Hash, class Equal >
karpovich::HashConstIter< Key, Value, Hash, Equal >::HashConstIter(const Vector< List< valType > > *data,
                                                                   size_t capacity, size_t start_idx):
  data_(data),
  capacity_(capacity),
  idx_(start_idx),
  listIt_(nullptr),
  listEnd_(nullptr)
{
  findValid();
}

template < class Key, class Value, class Hash, class Equal >
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
}

template < class Key, class Value, class Hash, class Equal >
karpovich::HashConstIter< Key, Value, Hash, Equal > &karpovich::HashConstIter< Key, Value, Hash, Equal >::operator++()
{
  ++listIt_;
  findValid();
  return *this;
}

template < class Key, class Value, class Hash, class Equal >
bool karpovich::HashConstIter< Key, Value, Hash, Equal >::operator==(const HashConstIter &other) const
{
  if (data_ == nullptr && other.data_ == nullptr) {
    return true;
  }
  if (data_ == nullptr || other.data_ == nullptr) {
    return false;
  }
  if (idx_ >= capacity_ && other.idx_ >= other.capacity_) {
    return true;
  }
  return idx_ == other.idx_ && listIt_ == other.listIt_;
}

template < class Key, class Value, class Hash, class Equal >
bool karpovich::HashConstIter< Key, Value, Hash, Equal >::operator!=(const HashConstIter &other) const
{
  return !(*this == other);
}

template < class Key, class Value, class Hash, class Equal >
const typename karpovich::HashConstIter< Key, Value, Hash, Equal >::valType &
karpovich::HashConstIter< Key, Value, Hash, Equal >::operator*() const
{
  return *listIt_;
}

#endif
