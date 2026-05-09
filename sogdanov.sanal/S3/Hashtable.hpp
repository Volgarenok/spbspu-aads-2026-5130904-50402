#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include "HashIter.hpp"
#include "HashFunc.hpp"
#include <stdexcept>
#include <utility>

namespace sogdanov
{
  enum class HashState
  {
    EMPTY,
    OCCUPIED,
    TOMBSTONE
  };
  template <class Key, class Value>
  struct HashNode
  {
    Key k;
    Value v;
    HashState state = HashState::EMPTY;

    bool is_occupied() const;
  };
  template <class Key, class Value, class Hash = XXHashFunctor, class Equal = EqualFunctor<Key>>
  class HashTable
  {
  private:
    HashNode<Key, Value> *table_;
    size_t capacity_;
    size_t size_;
    size_t tombstones_;
    Hash hash_fn_;
    Equal eq_fn_;

  public:
    using Iterator = HashIter<HashNode<Key, Value>>;

    explicit HashTable(size_t cap = 0);
    HashTable(const HashTable &other);
    HashTable &operator=(HashTable other);
    ~HashTable();

    void swap(HashTable &other) noexcept;

    void add(Key k, Value v);
    Value drop(Key k);
    bool has(Key k) const;
    Value &get(Key k);

    void rehash(size_t slots);
    size_t size() const;

    Iterator begin();
    Iterator end();
  };
}
template <class Key, class Value>
bool sogdanov::HashNode<Key, Value>::is_occupied() const
{
  return state == HashState::OCCUPIED;
}

template <class Key, class Value, class Hash, class Equal>
sogdanov::HashTable<Key, Value, Hash, Equal>::HashTable(size_t cap)
    : table_(nullptr), capacity_(0), size_(0), tombstones_(0)
{
  if (cap > 0)
  {
    rehash(cap);
  }
}

template <class Key, class Value, class Hash, class Equal>
sogdanov::HashTable<Key, Value, Hash, Equal>::HashTable(const HashTable &other)
    : table_(nullptr), capacity_(0), size_(0), tombstones_(0)
{
  if (other.capacity_ > 0)
  {
    rehash(other.capacity_);
    for (size_t i = 0; i < other.capacity_; ++i)
    {
      if (other.table_[i].state == HashState::OCCUPIED)
      {
        add(other.table_[i].k, other.table_[i].v);
      }
    }
  }
}

template <class Key, class Value, class Hash, class Equal>
sogdanov::HashTable<Key, Value, Hash, Equal> &
sogdanov::HashTable<Key, Value, Hash, Equal>::operator=(HashTable other)
{
  swap(other);
  return *this;
}

template <class Key, class Value, class Hash, class Equal>
sogdanov::HashTable<Key, Value, Hash, Equal>::~HashTable()
{
  delete[] table_;
}

template <class Key, class Value, class Hash, class Equal>
void sogdanov::HashTable<Key, Value, Hash, Equal>::swap(HashTable &other) noexcept
{
  std::swap(table_, other.table_);
  std::swap(capacity_, other.capacity_);
  std::swap(size_, other.size_);
  std::swap(tombstones_, other.tombstones_);
}

template <class Key, class Value, class Hash, class Equal>
void sogdanov::HashTable<Key, Value, Hash, Equal>::add(Key k, Value v)
{
  if (size_ + tombstones_ >= capacity_)
  {
    throw std::overflow_error("HashTable is full, manual rehash required");
  }

  size_t h = hash_fn_(k) % capacity_;
  size_t i = 0;
  size_t first_tombstone = capacity_;

  while (table_[(h + i * i) % capacity_].state != HashState::EMPTY)
  {
    size_t idx = (h + i * i) % capacity_;
    if (table_[idx].state == HashState::OCCUPIED && eq_fn_(table_[idx].k, k))
    {
      table_[idx].v = v;
      return;
    }
    if (table_[idx].state == HashState::TOMBSTONE && first_tombstone == capacity_)
    {
      first_tombstone = idx;
    }
    ++i;
  }

  size_t target_idx = (first_tombstone != capacity_) ? first_tombstone : (h + i * i) % capacity_;

  if (table_[target_idx].state == HashState::TOMBSTONE)
  {
    tombstones_--;
  }
  table_[target_idx].k = k;
  table_[target_idx].v = std::move(v);
  table_[target_idx].state = HashState::OCCUPIED;
  size_++;
}

template <class Key, class Value, class Hash, class Equal>
Value sogdanov::HashTable<Key, Value, Hash, Equal>::drop(Key k)
{
  if (capacity_ == 0)
  {
    throw std::out_of_range("Key not found");
  }
  size_t h = hash_fn_(k) % capacity_;
  size_t i = 0;

  while (table_[(h + i * i) % capacity_].state != HashState::EMPTY)
  {
    size_t idx = (h + i * i) % capacity_;
    if (table_[idx].state == HashState::OCCUPIED && eq_fn_(table_[idx].k, k))
    {
      Value ret = std::move(table_[idx].v);
      table_[idx].state = HashState::TOMBSTONE;
      size_--;
      tombstones_++;
      return ret;
    }
    ++i;
    if (i > capacity_)
    {
      break;
    }
  }
  throw std::out_of_range("Key not found");
}

template <class Key, class Value, class Hash, class Equal>
bool sogdanov::HashTable<Key, Value, Hash, Equal>::has(Key k) const
{
  if (capacity_ == 0)
  {
    return false;
  }
  size_t h = hash_fn_(k) % capacity_;
  size_t i = 0;

  while (table_[(h + i * i) % capacity_].state != HashState::EMPTY)
  {
    size_t idx = (h + i * i) % capacity_;
    if (table_[idx].state == HashState::OCCUPIED && eq_fn_(table_[idx].k, k))
    {
      return true;
    }
    ++i;
    if (i > capacity_)
    {
      break;
    }
  }
  return false;
}

template <class Key, class Value, class Hash, class Equal>
Value &sogdanov::HashTable<Key, Value, Hash, Equal>::get(Key k)
{
  if (capacity_ == 0)
  {
    throw std::out_of_range("Key not found");
  }
  size_t h = hash_fn_(k) % capacity_;
  size_t i = 0;

  while (table_[(h + i * i) % capacity_].state != HashState::EMPTY)
  {
    size_t idx = (h + i * i) % capacity_;
    if (table_[idx].state == HashState::OCCUPIED && eq_fn_(table_[idx].k, k))
    {
      return table_[idx].v;
    }
    ++i;
    if (i > capacity_)
    {
      break;
    }
  }
  throw std::out_of_range("Key not found");
}

template <class Key, class Value, class Hash, class Equal>
void sogdanov::HashTable<Key, Value, Hash, Equal>::rehash(size_t slots)
{
  if (slots < size_)
  {
    throw std::invalid_argument("New capacity too small");
  }
  HashNode<Key, Value> *old_table = table_;
  size_t old_cap = capacity_;

  table_ = new HashNode<Key, Value>[slots];
  capacity_ = slots;
  size_ = 0;
  tombstones_ = 0;

  for (size_t i = 0; i < old_cap; ++i)
  {
    if (old_table[i].state == HashState::OCCUPIED)
    {
      add(old_table[i].k, old_table[i].v);
    }
  }
  delete
}

template <class Key, class Value, class Hash, class Equal>
size_t sogdanov::HashTable<Key, Value, Hash, Equal>::size() const
{
  return size_;
}

template <class Key, class Value, class Hash, class Equal>
typename sogdanov::HashTable<Key, Value, Hash, Equal>::Iterator
sogdanov::HashTable<Key, Value, Hash, Equal>::begin()
{
  return Iterator(table_, capacity_, 0);
}

template <class Key, class Value, class Hash, class Equal>
typename sogdanov::HashTable<Key, Value, Hash, Equal>::Iterator
sogdanov::HashTable<Key, Value, Hash, Equal>::end()
{
  return Iterator(table_, capacity_, capacity_);
}

#endif
