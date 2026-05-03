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
    LIter< Value > list_it_;
    LIter< Value > list_end_;

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
    HashConstIter(Vector< List< valType > > *data, size_t capacity, size_t start_idx);

    HashConstIter &operator++();
    bool operator==(const HashConstIter &other) const;
    bool operator!=(const HashConstIter &other) const;
    valType &operator*() const;

  private:
    Vector< List< valType > > *data_;
    size_t capacity_;
    size_t idx_;
    LIter< Value > list_it_;
    LIter< Value > list_end_;

    void findValid();
  };
}

template < class Key, class Value, class Hash, class Equal >
karpovich::HashIter< Key, Value, Hash, Equal >::HashIter():
  data_(nullptr),
  capacity_(0),
  idx_(0),
  list_it_(),
  list_end_()
{}

template < class Key, class Value, class Hash, class Equal >
karpovich::HashIter< Key, Value, Hash, Equal >::HashIter(Vector< List< valType > > *data, size_t capacity,
                                                         size_t start_idx):
  data_(data),
  capacity_(capacity),
  idx_(start_idx),
  list_it_(),
  list_end_()
{
  findValid();
}

template < class Key, class Value, class Hash, class Equal >
void karpovich::HashIter< Key, Value, Hash, Equal >::findValid()
{
  while (idx_ < capacity_) {
    list_it_ = (*data_)[idx_].begin();
    list_end_ = (*data_)[idx_].end();
    if (list_it_ != list_end_) {
      return;
    }
    ++idx_;
  }
}

template < class Key, class Value, class Hash, class Equal >
karpovich::HashIter< Key, Value, Hash, Equal > &karpovich::HashIter< Key, Value, Hash, Equal >::operator++()
{
  ++list_it_;
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
  return idx_ == other.idx_ && list_it_ == other.list_it_;
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
  return *list_it_;
}

template < class Key, class Value, class Hash, class Equal >
karpovich::HashConstIter< Key, Value, Hash, Equal >::HashConstIter():
  data_(nullptr),
  capacity_(0),
  idx_(0),
  list_it_(),
  list_end_()
{}

#endif
