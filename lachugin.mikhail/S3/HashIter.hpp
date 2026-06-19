#ifndef HASHITER_HPP
#define HASHITER_HPP

#include <cstddef>
#include "HashItem.hpp"
namespace lachugin
{
  template< class Key, class Value, class Hash, class Equal >
  class HashTable;

  template< class Key, class Value, class Hash, class Equal >
  class HashIter
  {
  public:
    friend class HashTable< Key, Value, Hash, Equal >;
    HashItem< Key, Value >& operator*() const;
    HashItem< Key, Value >* operator->() const;

    HashIter& operator++();
    bool operator==(const HashIter& other) const;
    bool operator!=(const HashIter& other) const;
  private:
    HashIter(HashTable< Key, Value, Hash, Equal >* table,size_t pos);
    void skipEmpty();
    HashTable< Key, Value, Hash, Equal >* table_;
    size_t pos_;
  };

  template< class Key, class Value, class Hash, class Equal >
  class HashConstIter
  {
    friend class HashTable< Key, Value, Hash, Equal >;
  public:
    const HashItem< Key, Value >& operator*() const;
    const HashItem< Key, Value >* operator->() const;

    HashConstIter& operator++();
    bool operator==(const HashConstIter& other) const;
    bool operator!=(const HashConstIter& other) const;
  private:
    HashConstIter(const HashTable< Key, Value, Hash, Equal >* table, size_t pos);
    void skipEmpty();
    const HashTable< Key, Value, Hash, Equal >* table_;
    size_t pos_;
  };

  template< class Key, class Value, class Hash, class Equal >
  HashIter< Key, Value, Hash, Equal >::HashIter(HashTable< Key, Value, Hash, Equal >* table,
    size_t pos):
  table_(table),
  pos_(pos)
  {
    skipEmpty();
  }

  template< class Key, class Value, class Hash, class Equal >
  HashItem< Key, Value >& HashIter< Key, Value, Hash, Equal >::operator*() const
  {
    return table_->data_[pos_];
  }

  template< class Key, class Value, class Hash, class Equal >
  HashItem< Key, Value >* HashIter< Key, Value, Hash, Equal >::operator->() const
  {
    return &(table_->data_[pos_]);
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashIter< Key, Value, Hash, Equal >::skipEmpty()
  {
    size_t cap = table_->bucketCount_ * table_->bucketCapacity_ + table_->spareCapacity_;
    while (pos_ < cap && !table_->data_[pos_].occupied)
    {
      ++pos_;
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  HashIter< Key, Value, Hash, Equal >& HashIter< Key, Value, Hash, Equal >::operator++()
  {
    ++pos_;
    skipEmpty();
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HashIter< Key, Value, Hash, Equal >::operator==(const HashIter& other) const
  {
    return table_ == other.table_ && pos_ == other.pos_;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HashIter< Key, Value, Hash, Equal >::operator!=(const HashIter& other) const
  {
    return !(*this == other);
  }


  template< class Key, class Value, class Hash, class Equal >
  HashConstIter< Key, Value, Hash, Equal >::HashConstIter(
  const HashTable< Key, Value, Hash, Equal >* table,
    size_t pos):
  table_(table),
  pos_(pos)
  {
    skipEmpty();
  }

  template< class Key, class Value, class Hash, class Equal >
  const HashItem< Key, Value >& HashConstIter< Key, Value, Hash, Equal >::operator*() const
  {
    return table_->data_[pos_];
  }

  template< class Key, class Value, class Hash, class Equal >
  const HashItem< Key, Value >* HashConstIter< Key, Value, Hash, Equal >::operator->() const
  {
    return &(table_->data_[pos_]);
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashConstIter< Key, Value, Hash, Equal >::skipEmpty()
  {
    size_t cap = table_->bucketCount_ * table_->bucketCapacity_ + table_->spareCapacity_;
    while (pos_ < cap && !table_->data_[pos_].occupied)
    {
      ++pos_;
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  HashConstIter< Key, Value, Hash, Equal >& HashConstIter< Key, Value, Hash, Equal >::operator++()
  {
    ++pos_;
    skipEmpty();
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HashConstIter< Key, Value, Hash, Equal >::operator==(const HashConstIter& other) const
  {
    return table_ == other.table_ && pos_ == other.pos_;
  }


  template< class Key, class Value, class Hash, class Equal >
  bool HashConstIter< Key, Value, Hash, Equal >::operator!=(const HashConstIter& other) const
  {
    return !(*this == other);
  }



}
#endif
