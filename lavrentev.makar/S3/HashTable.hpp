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
      //HashTable(const HashTable &);
      //HashTable(HashTable &&);
      ~HashTable() noexcept;
      //HashTable<Key, Value, Hash, Equal> &operator=(const HashTable<Key, Value, Hash, Equal> &);
      //HashTable<Key, Value, Hash, Equal> &operator=(HashTable<Key, Value, Hash, Equal> &&);

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
void lavrentev::HashTable<Key, Value, Hash, Equal>::add(Key k, Value v) {
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
Value lavrentev::HashTable<Key, Value, Hash, Equal>::drop(Key k) {
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
size_t lavrentev::HashTable<Key, Value, Hash, Equal>::size() {
  return size_;
}

#endif
