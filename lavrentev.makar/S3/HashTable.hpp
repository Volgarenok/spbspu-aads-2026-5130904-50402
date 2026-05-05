#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP
#include <cstddef>
#include "../common/List.hpp"

namespace lavrentev {
  template< class Key, class Value, class Hash, class Equal >
  class HashTable {
    public:
      void add(Key k, Value v);
      Value drop(Key k);
      bool has(Key k);
      void rehash(size_t slots);
    private:
      List<std::pair<Key, Value>> ht;
  };
}

#endif
