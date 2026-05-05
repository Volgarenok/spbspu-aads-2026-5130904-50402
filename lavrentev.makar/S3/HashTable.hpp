#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP
#include <cstddef>
#include <stdexcept>
#include "../common/List.hpp"

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

      void add(Key k, Value v);
      Value drop(Key k);
      bool has(Key k);
      void rehash(size_t slots);
    private:
      List<Key> htKeys;
      List<Value> htValues;
  };
}

template< class Key, class Value, class Hash, class Equal >
void lavrentev::HashTable<Key, Value, Hash, Equal>::add(Key k, Value v) {
  Key current = htKeys.cbegin();
  Key end = nullptr;
  while (current->next != nullptr) {
    if (current->val == k) {
      throw std::out_of_range("Key already exists");
    }
    end = current->next;
  }
  htKeys.insert(end, v);
}

#endif
