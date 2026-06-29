#ifndef CUCKOO_ITERS_HPP
#define CUCKOO_ITERS_HPP

#include <Vector.hpp>
#include <cstddef>
#include <utility>
#include "cuckooEntry.hpp"

namespace karpovich
{
  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  class CuckooTable;
  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  class CuckooIter;
  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  class CuckooConstIter;

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  class CuckooIter
  {
    friend class CuckooTable< Key, Value, Hash1, Hash2, Equal >;
    friend class CuckooConstIter< Key, Value, Hash1, Hash2, Equal >;
    using valType = std::pair< Key, Value >;
    using entryType = details::CuckooEntry< Key, Value >;

  public:
    CuckooIter();
    CuckooIter(Vector< entryType > *table1, Vector< entryType > *table2, size_t capacity, size_t tableIdx, size_t idx);
    CuckooIter &operator++();
    CuckooIter operator++(int);
    bool operator==(const CuckooIter &other) const;
    bool operator!=(const CuckooIter &other) const;
    valType &operator*() const;
    valType *operator->() const;

  private:
    Vector< entryType > *table1_;
    Vector< entryType > *table2_;
    size_t capacity_;
    size_t tableIdx_;
    size_t idx_;
    void findValid();
  };

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  class CuckooConstIter
  {
    friend class CuckooTable< Key, Value, Hash1, Hash2, Equal >;
    friend class CuckooIter< Key, Value, Hash1, Hash2, Equal >;

    using valType = std::pair< Key, Value >;
    using entryType = details::CuckooEntry< Key, Value >;

  public:
    CuckooConstIter();
    CuckooConstIter(const Vector< entryType > *table1, const Vector< entryType > *table2, size_t capacity,
                    size_t tableIdx, size_t idx);
    CuckooConstIter &operator++();
    CuckooConstIter operator++(int);
    bool operator==(const CuckooConstIter &other) const;
    bool operator!=(const CuckooConstIter &other) const;
    const valType &operator*() const;
    const valType *operator->() const;

  private:
    const Vector< entryType > *table1_;
    const Vector< entryType > *table2_;
    size_t capacity_;
    size_t tableIdx_;
    size_t idx_;
    void findValid();
  };
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
karpovich::CuckooIter< Key, Value, Hash1, Hash2, Equal >::CuckooIter():
  table1_(nullptr),
  table2_(nullptr),
  capacity_(0),
  tableIdx_(2),
  idx_(0)
{}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
karpovich::CuckooIter< Key, Value, Hash1, Hash2, Equal >::CuckooIter(Vector< entryType > *table1,
                                                                     Vector< entryType > *table2, size_t capacity,
                                                                     size_t tableIdx, size_t idx):
  table1_(table1),
  table2_(table2),
  capacity_(capacity),
  tableIdx_(tableIdx),
  idx_(idx)
{
  findValid();
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
void karpovich::CuckooIter< Key, Value, Hash1, Hash2, Equal >::findValid()
{
  while (tableIdx_ < 2) {
    Vector< entryType > *current = (tableIdx_ == 0) ? table1_ : table2_;
    while (idx_ < capacity_) {
      if ((*current)[idx_].occupied_) {
        return;
      }
      ++idx_;
    }
    ++tableIdx_;
    idx_ = 0;
  }

  table1_ = nullptr;
  table2_ = nullptr;
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
karpovich::CuckooIter< Key, Value, Hash1, Hash2, Equal > &
karpovich::CuckooIter< Key, Value, Hash1, Hash2, Equal >::operator++()
{
  ++idx_;
  findValid();
  return *this;
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
karpovich::CuckooIter< Key, Value, Hash1, Hash2, Equal >
karpovich::CuckooIter< Key, Value, Hash1, Hash2, Equal >::operator++(int)
{
  CuckooIter temp(*this);
  ++(*this);
  return temp;
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
bool karpovich::CuckooIter< Key, Value, Hash1, Hash2, Equal >::operator==(const CuckooIter &other) const
{
  bool thisEnd = (table1_ == nullptr || tableIdx_ >= 2);
  bool otherEnd = (other.table1_ == nullptr || other.tableIdx_ >= 2);
  if (thisEnd && otherEnd) {
    return true;
  }
  if (thisEnd || otherEnd) {
    return false;
  }
  return tableIdx_ == other.tableIdx_ && idx_ == other.idx_;
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
bool karpovich::CuckooIter< Key, Value, Hash1, Hash2, Equal >::operator!=(const CuckooIter &other) const
{
  return !(*this == other);
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
typename karpovich::CuckooIter< Key, Value, Hash1, Hash2, Equal >::valType &
karpovich::CuckooIter< Key, Value, Hash1, Hash2, Equal >::operator*() const
{
  Vector< entryType > *current = (tableIdx_ == 0) ? table1_ : table2_;
  return (*current)[idx_].data_;
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
typename karpovich::CuckooIter< Key, Value, Hash1, Hash2, Equal >::valType *
karpovich::CuckooIter< Key, Value, Hash1, Hash2, Equal >::operator->() const
{
  return std::addressof(operator*());
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
karpovich::CuckooConstIter< Key, Value, Hash1, Hash2, Equal >::CuckooConstIter():
  table1_(nullptr),
  table2_(nullptr),
  capacity_(0),
  tableIdx_(2),
  idx_(0)
{}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
karpovich::CuckooConstIter< Key, Value, Hash1, Hash2, Equal >::CuckooConstIter(const Vector< entryType > *table1,
                                                                               const Vector< entryType > *table2,
                                                                               size_t capacity, size_t tableIdx,
                                                                               size_t idx):
  table1_(table1),
  table2_(table2),
  capacity_(capacity),
  tableIdx_(tableIdx),
  idx_(idx)
{
  findValid();
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
void karpovich::CuckooConstIter< Key, Value, Hash1, Hash2, Equal >::findValid()
{
  while (tableIdx_ < 2) {
    const Vector< entryType > *current = (tableIdx_ == 0) ? table1_ : table2_;
    while (idx_ < capacity_) {
      if ((*current)[idx_].occupied_) {
        return;
      }
      ++idx_;
    }
    ++tableIdx_;
    idx_ = 0;
  }
  table1_ = nullptr;
  table2_ = nullptr;
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
karpovich::CuckooConstIter< Key, Value, Hash1, Hash2, Equal > &
karpovich::CuckooConstIter< Key, Value, Hash1, Hash2, Equal >::operator++()
{
  ++idx_;
  findValid();
  return *this;
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
karpovich::CuckooConstIter< Key, Value, Hash1, Hash2, Equal >
karpovich::CuckooConstIter< Key, Value, Hash1, Hash2, Equal >::operator++(int)
{
  CuckooConstIter temp(*this);
  ++(*this);
  return temp;
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
bool karpovich::CuckooConstIter< Key, Value, Hash1, Hash2, Equal >::operator==(const CuckooConstIter &other) const
{
  bool thisEnd = (table1_ == nullptr || tableIdx_ >= 2);
  bool otherEnd = (other.table1_ == nullptr || other.tableIdx_ >= 2);
  if (thisEnd && otherEnd) {
    return true;
  }
  if (thisEnd || otherEnd) {
    return false;
  }
  return tableIdx_ == other.tableIdx_ && idx_ == other.idx_;
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
bool karpovich::CuckooConstIter< Key, Value, Hash1, Hash2, Equal >::operator!=(const CuckooConstIter &other) const
{
  return !(*this == other);
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
const typename karpovich::CuckooConstIter< Key, Value, Hash1, Hash2, Equal >::valType &
karpovich::CuckooConstIter< Key, Value, Hash1, Hash2, Equal >::operator*() const
{
  const Vector< entryType > *current = (tableIdx_ == 0) ? table1_ : table2_;

  return (*current)[idx_].data_;
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
const typename karpovich::CuckooConstIter< Key, Value, Hash1, Hash2, Equal >::valType *
karpovich::CuckooConstIter< Key, Value, Hash1, Hash2, Equal >::operator->() const
{
  return std::addressof(operator*());
}

#endif
