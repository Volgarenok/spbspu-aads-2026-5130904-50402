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
      HashTable(HashTable &&);
      ~HashTable() noexcept;
      HashTable<Key, Value, Hash, Equal> &operator=(const HashTable<Key, Value, Hash, Equal> &);
      HashTable<Key, Value, Hash, Equal> &operator=(HashTable<Key, Value, Hash, Equal> &&);

      bool operator[](const HashTable< Key, Value, Hash, Equal > &other) const;

      void add(Key k, Value v);
      Value drop(Key k);
      bool has(Key k);
      void rehash(size_t slots);
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
void lavrentev::HashTable<Key, Value, Hash, Equal>::add(Key k, Value v) {
  size_t idx = Hash{}(k) % slots_;
  List<Node> &bucket = ht_[idx];
  for (LIter<Node> it = bucket.begin(); it != bucket.end(); ++it)
  {
    if(Equal{}(it->curr->val->key, k))
    {
      throw std::invalid_argument("Key already exists");
    }
  }
  bucket.pushFront(Node{k, v});
  ++size_;
}

#endif
