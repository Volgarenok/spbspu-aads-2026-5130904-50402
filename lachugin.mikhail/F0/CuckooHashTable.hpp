#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include "CuckooIter.hpp"
#include "CuckooItem.hpp"

#include <cstddef>
#include <functional>
#include <stdexcept>

namespace lachugin
{
  template< class Key >
  struct DefaultHash1
  {
    size_t operator()(const Key& key) const
    {
      return std::hash< Key >{}(key);
    }
  };

  template< class Key >
  struct DefaultHash2
  {
    size_t operator()(const Key& key) const
    {
      size_t h = std::hash< Key >{}(key);
      return h ^ 0x9e3779b97f4a7c15ULL;
    }
  };


  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  class CuckooIter;

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  class CuckooConstIter;

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  class CuckooHashTable
  {
  public:
    using value_type = std::pair< Key, Value >;
    friend class CuckooIter< Key, Value, Hash1, Hash2, Equal >;
    friend class CuckooConstIter< Key, Value, Hash1, Hash2, Equal >;

    CuckooHashTable(size_t capacity = 17);
    ~CuckooHashTable();

    CuckooHashTable(const CuckooHashTable&);
    CuckooHashTable& operator=(const CuckooHashTable&);
    CuckooHashTable(CuckooHashTable&&) noexcept;
    CuckooHashTable& operator=(CuckooHashTable&&) noexcept;

    void add(const Key&, const Value&);
    Value drop(const Key&);
    bool has(const Key&) const;
    Value& get(const Key&);
    const Value& get(const Key&) const;
    void clear();
    size_t size() const;
    bool empty() const;
    void rehash(size_t);

    CuckooIter< Key, Value, Hash1, Hash2, Equal > begin();
    CuckooIter< Key, Value, Hash1, Hash2, Equal > end();
    CuckooConstIter< Key, Value, Hash1, Hash2, Equal > begin() const;
    CuckooConstIter< Key, Value, Hash1, Hash2, Equal > end() const;
  private:
    size_t hashPos1(const Key&) const;
    size_t hashPos2(const Key&) const;
    void insertWithoutCheck(const Key&, const Value&);
    void swap(CuckooHashTable& other);

    CuckooItem< Key, Value >* table1_;
    CuckooItem< Key, Value >* table2_;
    size_t capacity_;
    size_t size_;

    Hash1 hash1_;
    Hash2 hash2_;
    Equal equal_;
  };

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::CuckooHashTable(size_t capacity):
  table1_(new CuckooItem< Key, Value >[capacity]),
  table2_(new CuckooItem< Key, Value >[capacity]),
  capacity_(capacity),
  size_(0)
  {}

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::~CuckooHashTable()
  {
    delete[] table1_;
    delete[] table2_;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  size_t CuckooHashTable<Key, Value, Hash1, Hash2, Equal>::size() const
  {
    return size_;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  bool CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::empty() const
  {
    return size_ == 0;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  size_t CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::hashPos1(const Key& key) const
  {
    return hash1_(key) % capacity_;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  size_t CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::hashPos2(const Key& key) const
  {
    return hash2_(key) % capacity_;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  bool CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::has(const Key& key) const
  {
    size_t pos1 = hashPos1(key);
    if (table1_[pos1].occupied && equal_(table1_[pos1].data.first, key))
    {
      return true;
    }
    size_t pos2 = hashPos2(key);
    if (table2_[pos2].occupied && equal_(table2_[pos2].data.first, key))
    {
      return true;
    }
    return false;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  Value& CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::get(const Key& key)
  {
    size_t pos1 = hashPos1(key);
    if (table1_[pos1].occupied && equal_(table1_[pos1].data.first, key))
    {
      return table1_[pos1].data.second;
    }
    size_t pos2 = hashPos2(key);
    if (table2_[pos2].occupied && equal_(table2_[pos2].data.first, key))
    {
      return table2_[pos2].data.second;
    }
    throw std::out_of_range("Key does not exist");
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  const Value& CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::get(const Key& key) const
  {
    size_t pos1 = hashPos1(key);
    if (table1_[pos1].occupied && equal_(table1_[pos1].data.first, key))
    {
      return table1_[pos1].data.second;
    }
    size_t pos2 = hashPos2(key);
    if (table2_[pos2].occupied && equal_(table2_[pos2].data.first, key))
    {
      return table2_[pos2].data.second;
    }
    throw std::out_of_range("Key does not exist");
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  void CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::clear()
  {
    delete[] table1_;
    delete[] table2_;
    table1_ = new CuckooItem< Key, Value >[capacity_];
    table2_ = new CuckooItem< Key, Value >[capacity_];
    size_ = 0;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::CuckooHashTable(const CuckooHashTable& other):
  table1_(new CuckooItem<Key, Value>[other.capacity_]),
  table2_(new CuckooItem<Key, Value>[other.capacity_]),
  capacity_(other.capacity_),
  size_(other.size_),
  hash1_(other.hash1_),
  hash2_(other.hash2_),
  equal_(other.equal_)
  {
    for (size_t i = 0; i < capacity_; ++i)
    {
      table1_[i] = other.table1_[i];
      table2_[i] = other.table2_[i];
    }
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  void CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::swap(CuckooHashTable& other)
  {
    std::swap(table1_, other.table1_);
    std::swap(table2_, other.table2_);
    std::swap(capacity_, other.capacity_);
    std::swap(size_, other.size_);
    std::swap(hash1_, other.hash1_);
    std::swap(hash2_, other.hash2_);
    std::swap(equal_, other.equal_);
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >&
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::operator=(const CuckooHashTable& other)
  {
    if (this != &other)
    {
      CuckooHashTable temp(other);
      swap(temp);
    }
    return *this;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::CuckooHashTable(CuckooHashTable&& other) noexcept:
  table1_(other.table1_),
  table2_(other.table2_),
  capacity_(other.capacity_),
  size_(other.size_),
  hash1_(std::move(other.hash1_)),
  hash2_(std::move(other.hash2_)),
  equal_(std::move(other.equal_))
  {
    other.table1_ = nullptr;
    other.table2_ = nullptr;
    other.capacity_ = 0;
    other.size_ = 0;
  }


  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >&
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::operator=(CuckooHashTable&& other) noexcept
  {
    if (this != &other)
    {
      delete[] table1_;
      delete[] table2_;
      table1_ = other.table1_;
      table2_ = other.table2_;
      capacity_ = other.capacity_;
      size_ = other.size_;
      hash1_ = std::move(other.hash1_);
      hash2_ = std::move(other.hash2_);
      equal_ = std::move(other.equal_);
      other.table1_ = nullptr;
      other.table2_ = nullptr;
      other.capacity_ = 0;
      other.size_ = 0;
    }
    return *this;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  void CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::insertWithoutCheck(const Key& key, const Value& value)
  {
    value_type current(key, value);
    bool firstTable = true;
    size_t kickCount = 0;
    while (kickCount < capacity_)
    {
      if (firstTable)
      {
        size_t pos = hashPos1(current.first);
        if (!table1_[pos].occupied)
        {
          table1_[pos].data = current;
          table1_[pos].occupied = true;
          ++size_;
          return;
        }
        std::swap(current, table1_[pos].data);
        firstTable = false;
      }
      else
      {
        size_t pos = hashPos2(current.first);
        if (!table2_[pos].occupied)
        {
          table2_[pos].data = current;
          table2_[pos].occupied = true;
          ++size_;
          return;
        }
        std::swap(current, table2_[pos].data);
        firstTable = true;
      }
      ++kickCount;
    }
    rehash(capacity_ * 2 + 1);
    insertWithoutCheck(current.first, current.second);
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  void CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::rehash(size_t newCapacity)
  {
    CuckooItem< Key, Value >* oldTable1 = table1_;
    CuckooItem< Key, Value >* oldTable2 = table2_;
    size_t oldCapacity = capacity_;


    table1_ = new CuckooItem< Key, Value >[newCapacity];
    table2_ = new CuckooItem< Key, Value >[newCapacity];
    capacity_ = newCapacity;
    size_t oldSize = size_;
    size_ = 0;


    try
    {
      for (size_t i = 0; i < oldCapacity; ++i)
      {
        if (oldTable1[i].occupied)
        {
          insertWithoutCheck(oldTable1[i].data.first, oldTable1[i].data.second);
        }
      }
      for (size_t i = 0; i < oldCapacity; ++i)
      {

        if (oldTable2[i].occupied)
        {
          insertWithoutCheck(oldTable2[i].data.first, oldTable2[i].data.second);
        }
      }
    }
    catch (...)
    {
      delete[] table1_;
      delete[] table2_;
      table1_ = oldTable1;
      table2_ = oldTable2;
      capacity_ = oldCapacity;
      size_ = oldSize;

      throw;
    }
    delete[] oldTable1;
    delete[] oldTable2;
  }


  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  void CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::add(const Key& key, const Value& value)
  {
    if (has(key))
    {
      throw std::logic_error("Key already exists");
    }
    insertWithoutCheck(key, value);
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  Value CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::drop(const Key& key)
  {
    size_t pos1 = hashPos1(key);
    if (table1_[pos1].occupied && equal_(table1_[pos1].data.first, key))
    {
      Value result = table1_[pos1].data.second;
      table1_[pos1].occupied = false;
      --size_;
      return result;
    }
    size_t pos2 = hashPos2(key);
    if (table2_[pos2].occupied && equal_(table2_[pos2].data.first, key))
    {
      Value result = table2_[pos2].data.second;
      table2_[pos2].occupied = false;
      --size_;
      return result;
    }
    throw std::out_of_range("Key does not exist");
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  CuckooIter< Key, Value, Hash1, Hash2, Equal >
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::begin()
  {
    CuckooIter< Key, Value, Hash1, Hash2, Equal > it(this,false,0);
    it.skipEmpty();
    return it;
  }

  template< class Key, class Value, class Hash1,class Hash2, class Equal >
  CuckooIter< Key, Value, Hash1, Hash2, Equal >
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::end()
  {
    return CuckooIter< Key, Value, Hash1, Hash2, Equal >(this, true, capacity_);
  }




}

#endif
