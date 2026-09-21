#ifndef HASHTABLEIMPL_HPP
#define HASHTABLEIMPL_HPP

#include <boost/hash2/siphash.hpp>
#include "HashTable.hpp"
#include <cstddef>
#include <stdexcept>
#include <utility>

namespace vishnevskiy
{
  template <class Key, class Value, class Hash, class Equal>
  size_t HashTable<Key, Value, Hash, Equal>::getIndex(const Key& key)
  {
    return hash(key) % cap;
  }

  template <class Key, class Value, class Hash, class Equal>
  size_t HashTable<Key, Value, Hash, Equal>::probe(size_t ind)
  {
    return (ind + 1) % cap;
  }

  template <class Key, class Value, class Hash, class Equal>
  size_t HashTable<Key, Value, Hash, Equal>::findByKey(const Key& key)
  {
    size_t ind = getIndex(key);
    for (size_t i = 0; i < cap; ++i)
    {
      if (flags[ind] == 0)
      {
        return cap;
      }
      if (flags[ind] == 1 && eq(keys[ind], key))
      {
        return ind;
      }
      ind = probe(ind);
    }
    return cap;
  }

  template <class Key, class Value, class Hash, class Equal>
  size_t HashTable<Key, Value, Hash, Equal>::findFree(const Key& key)
  {
    size_t ind = getIndex(key);
    size_t firstTombstone = cap;
    bool hasTombstone = false;

    for (size_t i = 0; i < cap; ++i)
    {
      size_t f = flags[ind];
      if (f == 0)
      {
        if (hasTombstone)
        {
          return firstTombstone;
        }
        return ind;
      }
      else if (f == 2 && !hasTombstone)
      {
        firstTombstone = ind;
        hasTombstone = true;
      }
      else if (f == 1 && eq(keys[ind], key))
      {
        return ind;
      }
      ind = probe(ind);
    }
    if (hasTombstone)
    {
      return firstTombstone;
    }
    return cap+1;
  }

  template <class Key, class Value, class Hash, class Equal>
  void HashTable<Key, Value, Hash, Equal>::add(const Key& k, const Value& v)
  {
    size_t curr = findFree(k);

    if (curr > cap)
    {
      throw std::runtime_error("Table is full");
    }
    if (flags[curr] == 1 && eq(keys[curr], k))
    {
      values[curr] = v;
    }
    else
    {
      keys[curr] = k;
      values[curr] = v;
      flags[curr] = 1;
      size++;
    }
  }

  template <class Key, class Value, class Hash, class Equal>
  bool HashTable<Key, Value, Hash, Equal>::drop(Key k, Value& res)
  {
    size_t curr = findByKey(k);

    if (curr < cap && flags[curr] == 1)
    {
      res = values[curr];
      flags[curr] = 2;
      size--;
      return true;
    }
    throw std::runtime_error("Key does not exist");
  }

  template <class Key, class Value, class Hash, class Equal>
  bool HashTable<Key, Value, Hash, Equal>::has(Key k)
  {
    return findByKey(k) < cap;
  }

  template <class Key, class Value, class Hash, class Equal>
  void HashTable<Key, Value, Hash, Equal>::rehash(size_t slots)
  {
    if (slots < size)
    {
      throw std::runtime_error("Not enough slots");
    }

    Key* newKeys = nullptr;
    Value* newValues = nullptr;
    size_t* newFlags = nullptr;
    try
    {
      newKeys = new Key[slots];
      newValues = new Value[slots];
      newFlags = new size_t[slots];
    }
    catch (const std::bad_alloc& )
    {
      delete[] newKeys;
      delete[] newValues;
      delete[] newFlags;
      throw;
    }

    for (size_t i = 0; i < slots; ++i)
    {
      newFlags[i] = 0;
    }
    Key* oldKeys = keys;
    Value* oldValues = values;
    size_t* oldFlags = flags;
    size_t oldCap = cap;
    size_t oldSize = size;

    keys = newKeys;
    values = newValues;
    flags = newFlags;
    cap = slots;
    size = 0;

    try
    {
      for (size_t i = 0; i < oldCap; ++i)
      {
        if (oldFlags[i] == 1)
        {
          add(oldKeys[i], oldValues[i]);
        }
      }
    }
    catch (...)
    {
      delete[] keys;
      delete[] values;
      delete[] flags;
      keys = oldKeys;
      values = oldValues;
      flags = oldFlags;
      cap = oldCap;
      size = oldSize;
      throw;
    }

    delete[] oldKeys;
    delete[] oldValues;
    delete[] oldFlags;
  }

  template <class Key, class Value, class Hash, class Equal>
  void HashTable<Key, Value, Hash, Equal>::createEls(size_t capacity)
  {
    Key* keyPtr = nullptr;
    Value* valPtr = nullptr;
    size_t* flagPtr = nullptr;

    try
    {
      keyPtr = new Key[capacity];
      valPtr = new Value[capacity];
      flagPtr = new size_t[capacity];
    }
    catch (const std::bad_alloc& e)
    {
      delete[] keyPtr;
      delete[] valPtr;
      delete[] flagPtr;
      throw;
    }

    for (size_t i = 0; i < capacity; ++i)
    {
      flagPtr[i] = 0;
    }
    keys = keyPtr;
    values = valPtr;
    flags = flagPtr;
  }

  template <class Key, class Value, class Hash, class Equal>
  HashTable<Key, Value, Hash, Equal>::HashTable(size_t capacity, Hash hash_f, Equal eq_f):
    keys(nullptr),
    values(nullptr),
    flags(nullptr),
    hash(hash_f),
    eq(eq_f),
    size(0),
    cap(capacity)
  {
    createEls(cap);
  }

  template <class Key, class Value, class Hash, class Equal>
  HashTable<Key, Value, Hash, Equal>::~HashTable()
  {
    delete[] keys;
    delete[] values;
    delete[] flags;
  }

  template <class Key, class Value, class Hash, class Equal>
  HashTable<Key, Value, Hash, Equal>::HashTable(const HashTable& other):
    keys(nullptr),
    values(nullptr),
    flags(nullptr),
    hash(other.hash),
    eq(other.eq),
    size(other.size),
    cap(other.cap)
  {
    createEls(cap);

    try
    {
      for (size_t i = 0; i < cap; ++i)
      {
        flags[i] = other.flags[i];
        if (flags[i] == 1)
        {
          keys[i] = other.keys[i];
          values[i] = other.values[i];
        }
      }
    }
    catch (...)
    {
      delete[] keys;
      delete[] values;
      delete[] flags;
      throw;
    }
  }

  template <class Key, class Value, class Hash, class Equal>
  HashTable<Key, Value, Hash, Equal>&
  HashTable<Key, Value, Hash, Equal>::operator=(HashTable other)
  {
    Key* tempKeys = keys;
    keys = other.keys;
    other.keys = tempKeys;

    Value* tempValues = values;
    values = other.values;
    other.values = tempValues;

    size_t* tempFlags = flags;
    flags = other.flags;
    other.flags = tempFlags;

    size_t tempSize = size;
    size = other.size;
    other.size = tempSize;

    size_t tempCap = cap;
    cap = other.cap;
    other.cap = tempCap;

    Hash tempHash = hash;
    hash = other.hash;
    other.hash = tempHash;

    Equal tempEq = eq;
    eq = other.eq;
    other.eq = tempEq;

    return *this;
  }

  template <class Key, class Value, class Hash, class Equal>
  size_t HashTable<Key, Value, Hash, Equal>::getSize() const
  {
    return size;
  }

  template <class Key, class Value, class Hash, class Equal>
  size_t HashTable<Key, Value, Hash, Equal>::getCapacity() const
  {
    return cap;
  }

  template <class Key, class Value, class Hash, class Equal>
  Value& HashTable<Key, Value, Hash, Equal>::at(const Key& k)
  {
    size_t curr = findByKey(k);

    if (curr < cap && flags[curr] == 1)
    {
      return values[curr];
    }
    throw std::runtime_error("Key does not exist");
  }

  template <class Key, class Value, class Hash, class Equal>
  const Value& HashTable<Key, Value, Hash, Equal>::at(const Key& k) const
  {
    size_t curr = findByKey(k);

    if (curr < cap && flags[curr] == 1)
    {
      return values[curr];
    }
    throw std::runtime_error("Key does not exist");
  }

  template <class Key, class Value, class Hash, class Equal>
  tableIt<Key, Value, Hash, Equal> HashTable<Key, Value, Hash, Equal>::begin() const
  {
    return tableIt<Key, Value, Hash, Equal>(this);
  }

  template <class Key, class Value, class Hash, class Equal>
  tableIt<Key, Value, Hash, Equal> HashTable<Key, Value, Hash, Equal>::end() const
  {
    return tableIt<Key, Value, Hash, Equal>(this, cap);
  }

  template <class Key, class Value, class Hash, class Equal>
  tableIt<Key, Value, Hash, Equal>::tableIt():
    table(nullptr),
    curr(0)
  {}

  template <class Key, class Value, class Hash, class Equal>
  tableIt<Key, Value, Hash, Equal>::tableIt(const HashTable<Key, Value, Hash, Equal>* InitTable):
    table(InitTable),
    curr(0)
  {
    while (curr < table->getCapacity() && table->flags[curr] != 1)
    {
      curr++;
    }
  }

  template <class Key, class Value, class Hash, class Equal>
  tableIt<Key, Value, Hash, Equal>::tableIt(const HashTable<Key, Value, Hash, Equal>* InitialTable, size_t index):
    table(InitialTable),
    curr(index)
  {}

  template <class Key, class Value, class Hash, class Equal>
  tableIt<Key, Value, Hash, Equal>& tableIt<Key, Value, Hash, Equal>::operator++()  {
    if (table && curr < table->cap)
    {
      curr++;
      while (curr < table->cap && table->flags[curr] != 1)
      {
        curr++;
      }
    }
    return *this;
  }

  template <class Key, class Value, class Hash, class Equal>
  tableIt<Key, Value, Hash, Equal> tableIt<Key, Value, Hash, Equal>::operator++(int)
  {
    tableIt temp = *this;
    ++(*this);
    return temp;
  }

  template <class Key, class Value, class Hash, class Equal>
  bool tableIt<Key, Value, Hash, Equal>::operator==(const tableIt& other) const
  {
    return table == other.table && curr == other.curr;
  }

  template <class Key, class Value, class Hash, class Equal>
  bool tableIt<Key, Value, Hash, Equal>::operator!=(const tableIt& other) const
  {
    return !(*this == other);
  }

  template <class Key, class Value, class Hash, class Equal>
  std::pair<const Key&, Value&> tableIt<Key, Value, Hash, Equal>::operator*() const
  {
    return std::pair<const Key&, Value&>(table->keys[curr], table->values[curr]);
  }
}

#endif
