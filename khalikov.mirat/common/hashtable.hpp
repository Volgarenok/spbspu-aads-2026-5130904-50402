#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP
#include "htiter.hpp"
#include "htciter.hpp"
#include "slotstate.hpp"
#include <cstddef>
#include <utility>
#include <stdexcept>

namespace khalikov
{
  template < class Key, class Value, class Hash, class Equal > class HashTable
  {
  public:
    HashTable();
    HashTable(const HashTable &rhs);
    HashTable(HashTable &&rhs) noexcept;
    ~HashTable();

    HashTable &operator=(const HashTable &rhs);
    HashTable &operator=(HashTable &&rhs) noexcept;
    Value &operator[](const Key &key);

    void swap(HashTable &rhs) noexcept;
    size_t getSize() const noexcept;
    size_t getCap() const noexcept;
    bool isEmpty() const noexcept;

    Value &at(const Key &key);
    const Value &at(const Key &key) const;
    bool insert(const Key &key, const Value &val);
    bool remove(const Key &key);
    bool has(const Key &key) const;

    HTIter< Key, Value, Hash, Equal > find(const Key &key);
    HTCIter< Key, Value, Hash, Equal > find(const Key &key) const;

    HTIter< Key, Value, Hash, Equal > begin();
    HTCIter< Key, Value, Hash, Equal > cbegin() const;

    HTIter< Key, Value, Hash, Equal > end();
    HTCIter< Key, Value, Hash, Equal > cend() const;

  private:
    void rehash(const size_t new_cap);
    struct Slot
    {
      Key key;
      Value value;
      SlotState state = SlotState::EMPTY;
    };
    friend class HTIter< Key, Value, Hash, Equal >;
    friend class HTCIter< Key, Value, Hash, Equal >;
    size_t size_;
    size_t cap_;
    Slot *slots_;
    Hash hasher;
  };
}

template < class Key, class Value, class Hash, class Equal >
khalikov::HashTable< Key, Value, Hash, Equal >::HashTable():
  size_(0),
  cap_(8),
  slots_(new Slot[8])
{}

template < class Key, class Value, class Hash, class Equal >
khalikov::HashTable< Key, Value, Hash, Equal >::~HashTable()
{
  delete[] slots_;
}

template < class Key, class Value, class Hash, class Equal >
void khalikov::HashTable< Key, Value, Hash, Equal >::swap(HashTable &rhs) noexcept
{
  std::swap(size_, rhs.size_);
  std::swap(cap_, rhs.cap_);
  std::swap(slots_, rhs.slots_);
  std::swap(hasher, rhs.hasher);
}

template < class Key, class Value, class Hash, class Equal >
size_t khalikov::HashTable< Key, Value, Hash, Equal >::getSize() const noexcept
{
  return size_;
}

template < class Key, class Value, class Hash, class Equal >
size_t khalikov::HashTable< Key, Value, Hash, Equal >::getCap() const noexcept
{
  return cap_;
}

template < class Key, class Value, class Hash, class Equal >
bool khalikov::HashTable< Key, Value, Hash, Equal >::isEmpty() const noexcept
{
  return size_ == 0;
}

template < class Key, class Value, class Hash, class Equal >
khalikov::HashTable< Key, Value, Hash, Equal >::HashTable(const HashTable &rhs):
  size_(0),
  cap_(rhs.cap_),
  slots_(new Slot[rhs.cap_])
{
  for (size_t i = 0; i < rhs.cap_; ++i) {
    slots_[i].state = rhs.slots_[i].state;
    if (rhs.slots_[i].state == SlotState::OCCUPIED) {
      slots_[i].key = rhs.slots_[i].key;
      slots_[i].value = rhs.slots_[i].value;
      size_++;
    }
  }
}

template < class Key, class Value, class Hash, class Equal >
khalikov::HashTable< Key, Value, Hash, Equal >::HashTable(HashTable &&rhs) noexcept:
  size_(0),
  cap_(0),
  slots_(nullptr)
{
  this->swap(rhs);
}

template < class Key, class Value, class Hash, class Equal >
khalikov::HashTable< Key, Value, Hash, Equal > &
khalikov::HashTable< Key, Value, Hash, Equal >::operator=(const HashTable &rhs)
{
  if (this != std::addressof(rhs)) {
    HashTable temp(rhs);
    this->swap(temp);
  }
  return *this;
}

template < class Key, class Value, class Hash, class Equal >
khalikov::HashTable< Key, Value, Hash, Equal > &
khalikov::HashTable< Key, Value, Hash, Equal >::operator=(HashTable &&rhs) noexcept
{
  this->swap(rhs);
  return *this;
}

template < class Key, class Value, class Hash, class Equal >
khalikov::HTIter< Key, Value, Hash, Equal > khalikov::HashTable< Key, Value, Hash, Equal >::begin()
{
  for (size_t i = 0; i < cap_; ++i) {
    if (slots_[i].state == SlotState::OCCUPIED) {
      return HTIter< Key, Value, Hash, Equal >(*this, i);
    }
  }
  return end();
}

template < class Key, class Value, class Hash, class Equal >
khalikov::HTIter< Key, Value, Hash, Equal > khalikov::HashTable< Key, Value, Hash, Equal >::end()
{
  return HTIter< Key, Value, Hash, Equal >(*this, cap_);
}

template < class Key, class Value, class Hash, class Equal >
khalikov::HTCIter< Key, Value, Hash, Equal > khalikov::HashTable< Key, Value, Hash, Equal >::cbegin() const
{
  for (size_t i = 0; i < cap_; ++i) {
    if (slots_[i].state == SlotState::OCCUPIED) {
      return HTCIter< Key, Value, Hash, Equal >(*this, i);
    }
  }
  return cend();
}

template < class Key, class Value, class Hash, class Equal >
khalikov::HTCIter< Key, Value, Hash, Equal > khalikov::HashTable< Key, Value, Hash, Equal >::cend() const
{
  return HTCIter< Key, Value, Hash, Equal >(*this, cap_);
}

template < class Key, class Value, class Hash, class Equal >
khalikov::HTIter< Key, Value, Hash, Equal > khalikov::HashTable< Key, Value, Hash, Equal >::find(const Key &key)
{
  if (size_ == 0) {
    return end();
  }
  size_t hash = hasher(key);
  for (size_t i = 0; i < cap_; ++i) {
    size_t h = (hash + i * i) % cap_;
    if (slots_[h].state == SlotState::EMPTY) {
      break;
    }
    if (slots_[h].state == SlotState::OCCUPIED && Equal{}(slots_[h].key, key)) {
      return HTIter< Key, Value, Hash, Equal >(*this, h);
    }
  }
  return end();
}

template < class Key, class Value, class Hash, class Equal >
khalikov::HTCIter< Key, Value, Hash, Equal > khalikov::HashTable< Key, Value, Hash, Equal >::find(const Key &key) const
{
  if (size_ == 0) {
    return cend();
  }
  size_t hash = hasher(key);
  for (size_t i = 0; i < cap_; ++i) {
    size_t h = (hash + i * i) % cap_;
    if (slots_[h].state == SlotState::EMPTY) {
      break;
    }
    if (slots_[h].state == SlotState::OCCUPIED && Equal{}(slots_[h].key, key)) {
      return HTCIter< Key, Value, Hash, Equal >(*this, h);
    }
  }
  return cend();
}

template < class Key, class Value, class Hash, class Equal >
void khalikov::HashTable< Key, Value, Hash, Equal >::rehash(size_t new_cap)
{
  Slot *new_slots = new Slot[new_cap];
  for (size_t old_i = 0; old_i < cap_; ++old_i) {
    if (slots_[old_i].state == SlotState::OCCUPIED) {
      size_t hash = hasher(slots_[old_i].key);
      for (size_t i = 0; i < new_cap; ++i) {
        size_t h = (hash + i * i) % new_cap;
        if (new_slots[h].state == SlotState::EMPTY) {
          new_slots[h].key = std::move(slots_[old_i].key);
          new_slots[h].value = std::move(slots_[old_i].value);
          new_slots[h].state = SlotState::OCCUPIED;
          break;
        }
      }
    }
  }
  delete[] slots_;
  slots_ = new_slots;
  cap_ = new_cap;
}

template < class Key, class Value, class Hash, class Equal >
bool khalikov::HashTable< Key, Value, Hash, Equal >::insert(const Key &key, const Value &val)
{
  if (size_ * 2 > cap_) {
    rehash(cap_ * 2);
  }
  size_t hash = hasher(key);
  size_t tomb = cap_;
  for (size_t i = 0; i < cap_; ++i) {
    size_t h = (hash + i * i) % cap_;
    if (slots_[h].state == SlotState::EMPTY) {
      if (tomb == cap_) {
        tomb = h;
      }
      break;
    }
    if (slots_[h].state == SlotState::OCCUPIED && Equal{}(slots_[h].key, key)) {
      slots_[h].value = val;
      return false;
    }
    if (slots_[h].state == SlotState::TOMBSTONE && tomb == cap_) {
      tomb = h;
    }
  }
  if (tomb != cap_) {
    slots_[tomb].key = key;
    slots_[tomb].value = val;
    slots_[tomb].state = SlotState::OCCUPIED;
    size_++;
    return true;
  }
  return false;
}

template < class Key, class Value, class Hash, class Equal >
Value &khalikov::HashTable< Key, Value, Hash, Equal >::at(const Key &key)
{
  auto it = find(key);
  if (it == end()) {
    throw std::out_of_range("Key not found");
  }
  return it->value;
}

template < class Key, class Value, class Hash, class Equal >
const Value &khalikov::HashTable< Key, Value, Hash, Equal >::at(const Key &key) const
{
  auto it = find(key);
  if (it == cend()) {
    throw std::out_of_range("Key not found");
  }
  return it->value;
}

template < class Key, class Value, class Hash, class Equal >
Value &khalikov::HashTable< Key, Value, Hash, Equal >::operator[](const Key &key)
{
  auto it = find(key);
  if (it == end()) {
    insert(key, Value{});
    return find(key)->value;
  }
  return it->value;
}

template < class Key, class Value, class Hash, class Equal >
bool khalikov::HashTable< Key, Value, Hash, Equal >::remove(const Key &key)
{
  size_t hash = hasher(key);
  for (size_t i = 0; i < cap_; ++i) {
    size_t h = (hash + i * i) % cap_;
    if (slots_[h].state == SlotState::EMPTY) {
      return false;
    }
    if (slots_[h].state == SlotState::OCCUPIED && Equal{}(slots_[h].key, key)) {
      slots_[h].state = SlotState::TOMBSTONE;
      size_--;
      return true;
    }
  }
  return false;
}

template < class Key, class Value, class Hash, class Equal >
bool khalikov::HashTable< Key, Value, Hash, Equal >::has(const Key &key) const
{
  return find(key) != cend();
}

#endif
