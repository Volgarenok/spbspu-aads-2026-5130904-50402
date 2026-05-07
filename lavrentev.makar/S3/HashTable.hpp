#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP
#include <cstddef>
#include <stdexcept>
#include "../common/List.hpp"
#include "siphash/hasher.hpp"

namespace lavrentev {
  template< class Key, class Value, class Hash, class Equal >
  class HashTable {
    public:
      HashTable();
      HashTable(const HashTable &);
      HashTable(HashTable &&) noexcept;
      ~HashTable() noexcept;
      HashTable<Key, Value, Hash, Equal> &operator=(const HashTable<Key, Value, Hash, Equal> &);
      HashTable<Key, Value, Hash, Equal> &operator=(HashTable<Key, Value, Hash, Equal> &&) noexcept;
      Value& operator[](const Key &k);

      void add(Key k, Value v);
      Value drop(Key k);
      bool has(Key k);
      void rehash(size_t slots);
      size_t size();
    private:
      struct Node {
        Key key;
        Value value;
      };
      List<Node> *ht_;
      std::size_t slots_;
      std::size_t size_;
      Hash hasher_;
      Equal equal_;
  };
}

template< class Key, class Value, class Hash, class Equal >
lavrentev::HashTable<Key, Value, Hash, Equal>::HashTable():
  ht_(new List<Node>[5]),
  slots_(5),
  size_(0)
{}

template<class Key, class Value, class Hash, class Equal>
lavrentev::HashTable<Key, Value, Hash, Equal>::~HashTable() noexcept
{
  delete[] ht_;
}

template<class Key, class Value, class Hash, class Equal>
lavrentev::HashTable<Key, Value, Hash, Equal>::HashTable(const HashTable &other):
  ht_(new List<Node>[other.slots_]),
  slots_(other.slots_),
  size_(other.size_),
  hasher_(other.hasher_),
  equal_(other.equal_)
{
  for(size_t i = 0; i < slots_; ++i)
  {
    ht_[i] = other.ht_[i];
  }
}

template<class Key, class Value, class Hash, class Equal>
lavrentev::HashTable<Key, Value, Hash, Equal>::HashTable(HashTable&& other) noexcept:
  ht_(other.ht_),
  slots_(other.slots_),
  size_(other.size_),
  hasher_(std::move(other.hasher_)),
  equal_(std::move(other.equal_))
{
  other.ht_ = nullptr;
  other.slots_ = 0;
  other.size_ = 0;
}

template<class Key, class Value, class Hash, class Equal>
lavrentev::HashTable<Key, Value, Hash, Equal>&
lavrentev::HashTable<Key, Value, Hash, Equal>::operator=(const HashTable& other) {
  if (this != &other) {
      delete[] ht_;
      slots_ = other.slots_;
      size_ = other.size_;
      hasher_ = other.hasher_;
      equal_ = other.equal_;

      ht_ = new List<Node>[slots_];
      for (size_t i = 0; i < slots_; ++i) {
          ht_[i] = other.ht_[i];
      }
  }
  return *this;
}

template<class Key, class Value, class Hash, class Equal>
lavrentev::HashTable<Key, Value, Hash, Equal>&
lavrentev::HashTable<Key, Value, Hash, Equal>::operator=(HashTable&& other) noexcept {
  if (this != &other) {
      delete[] ht_;
      ht_ = other.ht_;
      slots_ = other.slots_;
      size_ = other.size_;
      hasher_ = std::move(other.hasher_);
      equal_ = std::move(other.equal_);

      other.ht_ = nullptr;
      other.slots_ = 0;
      other.size_ = 0;
  }
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
Value& lavrentev::HashTable<Key, Value, Hash, Equal>::operator[](const Key &k)
{
  size_t idx = hasher_(k) % slots_;
  List<Node> &bucket = ht_[idx];
  for (LIter<Node> it = bucket.begin(); it != bucket.end(); ++it)
  {
    if(Equal{}((*it).key, k))
    {
      return (*it).value;
    }
  }
  bucket.pushFront(Node{k, Value{}});
  ++size_;
  return bucket.front().value;
}

template< class Key, class Value, class Hash, class Equal >
void lavrentev::HashTable<Key, Value, Hash, Equal>::add(Key k, Value v)
{
  size_t idx = hasher_(k) % slots_;
  List<Node> &bucket = ht_[idx];
  for (LIter<Node> it = bucket.begin(); it != bucket.end(); ++it)
  {
    if(Equal{}((*it).key, k))
    {
      throw std::invalid_argument("Key already exists");
    }
  }
  bucket.pushFront(Node{k, v});
  ++size_;
}

template< class Key, class Value, class Hash, class Equal >
Value lavrentev::HashTable<Key, Value, Hash, Equal>::drop(Key k)
{
  size_t idx = hasher_(k) % slots_;
  List<Node> &bucket = ht_[idx];
  for (LIter<Node> it = bucket.begin(); it != bucket.end(); ++it)
  {
    if (equal_((*it).key, k))
    {
      Value out = (*it).value;
      bucket.erase(it);
      --size_;
      return out;
    }
  }
  throw std::invalid_argument("Key is not exists");
}

template< class Key, class Value, class Hash, class Equal >
bool lavrentev::HashTable<Key, Value, Hash, Equal>::has(Key k)
{
  size_t idx = hasher_(k) % slots_;
  List<Node> &bucket = ht_[idx];
  for (LIter<Node> it = bucket.begin(); it != bucket.end(); ++it)
  {
    if (equal_((*it).key, k))
    {
      return true;
    }
  }
  return false;
}

template< class Key, class Value, class Hash, class Equal >
size_t lavrentev::HashTable<Key, Value, Hash, Equal>::size()
{
  return size_;
}

#endif
