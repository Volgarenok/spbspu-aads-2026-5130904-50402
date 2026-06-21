#ifndef ITERATOR_HPP
#define ITERATOR_HPP
#include <algorithm>
#include "CuckooHashTable.hpp"

namespace lachugin
{
  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  class CuckooHashTable;

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  class CuckooIter
  {
  public:
    friend class CuckooHashTable< Key, Value, Hash1, Hash2, Equal >;
    std::pair< Key, Value >& operator*() const;
    std::pair< Key, Value> * operator->() const;
    CuckooIter& operator++();
    bool operator==(const CuckooIter&) const;
    bool operator!=(const CuckooIter&) const;

  private:
    CuckooIter(CuckooHashTable< Key, Value, Hash1, Hash2, Equal >* table, bool secondTable, size_t pos);
    void skipEmpty();
    CuckooHashTable< Key, Value, Hash1, Hash2, Equal >* table_;
    bool secondTable_;
    size_t pos_;
  };

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  class CuckooConstIter
  {
  public:
    friend class CuckooHashTable<Key, Value, Hash1, Hash2, Equal>;
    std::pair< Key, Value >& operator*() const;
    std::pair< Key, Value> * operator->() const;
    CuckooConstIter& operator++();
    bool operator==(const CuckooConstIter&) const;
    bool operator!=(const CuckooConstIter&) const;

  private:
    CuckooConstIter(CuckooHashTable< Key, Value, Hash1, Hash2, Equal >* table, bool secondTable, size_t pos);
    void skipEmpty();
    CuckooHashTable< Key, Value, Hash1, Hash2, Equal >* table_;
    bool secondTable_;
    size_t pos_;
  };


  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  CuckooIter< Key, Value, Hash1, Hash2, Equal >::CuckooIter(
    CuckooHashTable< Key, Value, Hash1, Hash2, Equal >* table, bool secondTable, size_t pos):
  table_(table),
  secondTable_(secondTable),
  pos_(pos)
  {}

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  std::pair< Key, Value >& CuckooIter< Key, Value, Hash1, Hash2, Equal >::operator*() const
  {
    if (!secondTable_)
    {
      return table_->table1_[pos_].data;
    }
    return table_->table2_[pos_].data;
  }

  template< class Key, class Value, class Hash1, class Hash2,class Equal >
  std::pair< Key, Value >* CuckooIter< Key, Value, Hash1, Hash2, Equal >::operator->() const
  {
    if (!secondTable_)
    {
      return &table_->table1_[pos_].data;
    }
    return &table_->table2_[pos_].data;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  bool CuckooIter< Key, Value, Hash1, Hash2, Equal >::operator==(const CuckooIter& other) const
  {
    return table_ == other.table_ && secondTable_ == other.secondTable_ && pos_ == other.pos_;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  bool CuckooIter< Key, Value, Hash1, Hash2, Equal >::operator!=(const CuckooIter& other) const
  {
    return !(*this == other);
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  void CuckooIter< Key, Value, Hash1, Hash2, Equal >::skipEmpty()
  {
    while (true)
    {
      if (!secondTable_)
      {
        while (pos_ < table_->capacity_ && !table_->table1_[pos_].occupied)
        {
          ++pos_;
        }
        if (pos_ < table_->capacity_)
        {
          return;
        }
        secondTable_ = true;
        pos_ = 0;
      }
      while (pos_ < table_->capacity_ && !table_->table2_[pos_].occupied)
      {
        ++pos_;
      }
      return;
    }
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  CuckooIter< Key, Value, Hash1, Hash2, Equal >&
    CuckooIter<Key, Value, Hash1, Hash2, Equal>::operator++()
  {
    ++pos_;
    skipEmpty();
    return *this;
  }




}
#endif
