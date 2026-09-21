#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <boost/hash2/siphash.hpp>
#include <utility>

namespace vishnevskiy
{
  template <class Key, class Value, class Hash, class Equal>
  class tableIt;

  template <class Key, class Value, class Hash, class Equal>
  class HashTable
  {
    private:
      Key* keys;
      Value* values;
      size_t* flags;
      Hash hash;
      Equal eq;
      size_t size;
      size_t cap;

      size_t getIndex(const Key& key);
      size_t probe(size_t ind);
      size_t findByKey(const Key& key);
      size_t findFree(const Key& key);
      void createEls(size_t capacity);

    public:
      HashTable(size_t capacity, Hash hash_f, Equal eq_f);
      HashTable(const HashTable& other);
      HashTable& operator=(HashTable other);
      ~HashTable();
      void add(const Key& k, const Value& v);
      bool drop(Key k, Value& res);
      Value& at(const Key& k);
      const Value& at(const Key& k) const;
      bool has(Key k);
      void rehash(size_t slots);
      size_t getSize() const;
      size_t getCapacity() const;

      tableIt<Key, Value, Hash, Equal> begin() const;
      tableIt<Key, Value, Hash, Equal> end() const;

      friend class tableIt<Key, Value, Hash, Equal>;
  };

  template <class Key, class Value, class Hash, class Equal>
  class tableIt
  {
    private:
      const HashTable<Key, Value, Hash, Equal>* table;
      size_t curr;
      tableIt(const HashTable<Key, Value, Hash, Equal>* table, size_t index);

    public:
      tableIt();
      tableIt(const HashTable<Key, Value, Hash, Equal>* InitTable);
      tableIt& operator++();
      tableIt operator++(int);
      bool operator==(const tableIt& other) const;
      bool operator!=(const tableIt& other) const;
      std::pair<const Key&, Value&> operator*() const;

      friend class HashTable<Key, Value, Hash, Equal>;
  };
}

#endif
