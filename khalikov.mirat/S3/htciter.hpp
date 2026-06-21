#ifndef HTCITER_HPP
#define HTCITER_HPP
#include <cstddef>
#include "slotstate.hpp"
namespace khalikov
{
  template < class Key, class Value, class Hash, class Equal > class HashTable;

  template < class Key, class Value, class Hash, class Equal > class HTCIter
  {
  public:
    HTCIter(const HashTable< Key, Value, Hash, Equal > &ht, size_t pos);
    bool operator==(const HTCIter &) const noexcept;
    bool operator!=(const HTCIter &) const noexcept;
    HTCIter &operator++();
    HTCIter operator++(int);
    const typename HashTable< Key, Value, Hash, Equal >::Slot &operator*() const;
    const typename HashTable< Key, Value, Hash, Equal >::Slot *operator->() const;

  private:
    size_t pos_;
    const HashTable< Key, Value, Hash, Equal > *table_;
  };
}

template < class Key, class Value, class Hash, class Equal >
khalikov::HTCIter< Key, Value, Hash, Equal >::HTCIter(const khalikov::HashTable< Key, Value, Hash, Equal > &ht,
                                                      size_t pos):
  pos_(pos),
  table_(&ht)
{}

template < class Key, class Value, class Hash, class Equal >
bool khalikov::HTCIter< Key, Value, Hash, Equal >::operator==(
    const HTCIter< Key, Value, Hash, Equal > &rhs) const noexcept
{
  return (rhs.table_ == table_ && rhs.pos_ == pos_);
}

template < class Key, class Value, class Hash, class Equal >
bool khalikov::HTCIter< Key, Value, Hash, Equal >::operator!=(
    const HTCIter< Key, Value, Hash, Equal > &rhs) const noexcept
{
  return !(*this == rhs);
}

template < class Key, class Value, class Hash, class Equal >
khalikov::HTCIter< Key, Value, Hash, Equal > &khalikov::HTCIter< Key, Value, Hash, Equal >::operator++()
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
const typename khalikov::HashTable< Key, Value, Hash, Equal >::Slot &
khalikov::HTCIter< Key, Value, Hash, Equal >::operator*() const
{
  return table_->slots_[pos_];
}

template < class Key, class Value, class Hash, class Equal >
const typename khalikov::HashTable< Key, Value, Hash, Equal >::Slot *
khalikov::HTCIter< Key, Value, Hash, Equal >::operator->() const
{
  return &table_->slots_[pos_];
}

template < class Key, class Value, class Hash, class Equal >
khalikov::HTCIter< Key, Value, Hash, Equal > khalikov::HTCIter< Key, Value, Hash, Equal >::operator++(int)
{
  HTCIter temp = *this;
  ++(*this);
  return temp;
}

#endif
