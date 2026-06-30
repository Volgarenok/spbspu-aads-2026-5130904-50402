#ifndef HTITER_HPP
#define HTITER_HPP
#include <cstddef>
#include "slotstate.hpp"

namespace khalikov
{
  template < class Key, class Value, class Hash, class Equal > class HashTable;

  template < class Key, class Value, class Hash, class Equal > class HTIter
  {
  public:
    HTIter(HashTable< Key, Value, Hash, Equal > &ht, size_t pos);
    bool operator==(const HTIter &) const noexcept;
    bool operator!=(const HTIter &) const noexcept;
    HTIter &operator++();
    HTIter operator++(int);
    typename HashTable< Key, Value, Hash, Equal >::Slot &operator*();
    typename HashTable< Key, Value, Hash, Equal >::Slot *operator->();

  private:
    size_t pos_;
    HashTable< Key, Value, Hash, Equal > *table_;
  };
}

template < class Key, class Value, class Hash, class Equal >
khalikov::HTIter< Key, Value, Hash, Equal >::HTIter(khalikov::HashTable< Key, Value, Hash, Equal > &ht, size_t pos):
  pos_(pos),
  table_(&ht)
{}

template < class Key, class Value, class Hash, class Equal >
bool khalikov::HTIter< Key, Value, Hash, Equal >::operator==(
    const HTIter< Key, Value, Hash, Equal > &rhs) const noexcept
{
  return (rhs.table_ == table_ && rhs.pos_ == pos_);
}

template < class Key, class Value, class Hash, class Equal >
bool khalikov::HTIter< Key, Value, Hash, Equal >::operator!=(
    const HTIter< Key, Value, Hash, Equal > &rhs) const noexcept
{
  return !(*this == rhs);
}

template < class Key, class Value, class Hash, class Equal >
khalikov::HTIter< Key, Value, Hash, Equal > &khalikov::HTIter< Key, Value, Hash, Equal >::operator++()
{
  if (pos_ < table_->cap_) {
    ++pos_;
    while (pos_ < table_->cap_ && table_->slots_[pos_].state != SlotState::OCCUPIED) {
      ++pos_;
    }
  }
  return *this;
}

template < class Key, class Value, class Hash, class Equal >
typename khalikov::HashTable< Key, Value, Hash, Equal >::Slot &khalikov::HTIter< Key, Value, Hash, Equal >::operator*()
{
  return table_->slots_[pos_];
}

template < class Key, class Value, class Hash, class Equal >
typename khalikov::HashTable< Key, Value, Hash, Equal >::Slot *khalikov::HTIter< Key, Value, Hash, Equal >::operator->()
{
  return &table_->slots_[pos_];
}

template < class Key, class Value, class Hash, class Equal >
khalikov::HTIter< Key, Value, Hash, Equal > khalikov::HTIter< Key, Value, Hash, Equal >::operator++(int)
{
  HTIter temp = *this;
  ++(*this);
  return temp;
}

#endif
