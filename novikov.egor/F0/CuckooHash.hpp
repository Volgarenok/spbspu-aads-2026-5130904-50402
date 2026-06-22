#ifndef CUCKOOHASH_HPP
#define CUCKOOHASH_HPP

#include <boost/container_hash/hash.hpp>
#include <functional>
#include <utility>
#include <algorithm>
#include <stdexcept>

namespace novikov
{
  template < class Key > struct Hash1
  {
    size_t operator()(const Key &key) const
    {
      return std::hash< Key >{}(key);
    }
  };

  template < class Key > struct Hash2
  {
    size_t operator()(const Key &key) const
    {
      return boost::hash< Key >{}(key);
    }
  };

  template < class Key, class Value, class Hash1 = Hash1< Key >, class Hash2 = Hash2< Key >,
             class Equal = std::equal_to< Key > >
  class CuckooHash
  {
  private:
    struct Item
    {
      std::pair< Key, Value > data;
      bool occupied;

      Item():
        data(),
        occupied(false)
      {}
    };
    Item *table1_;
    Item *table2_;
    size_t capacity_;
    size_t size_;
    Hash1 hash1_;
    Hash2 hash2_;
    Equal equal_;
    size_t hashPos1(const Key &key) const;
    size_t hashPos2(const Key &key) const;
    void swap(CuckooHash &other) noexcept;
    void insertWithoutCheck(const Key &key, const Value &value);

  public:
    CuckooHash(size_t capacity = 17);
    CuckooHash(const CuckooHash &other);
    CuckooHash(CuckooHash &&other) noexcept;
    ~CuckooHash();
    CuckooHash &operator=(const CuckooHash &other);
    CuckooHash &operator=(CuckooHash &&other) noexcept;
    void insert(const Key &key, const Value &value);
    Value get(const Key &key) const;
    bool has(const Key &key) const;
    bool remove(const Key &key);
    void clear();
    void rehash(size_t newCapacity);
    size_t size() const;
    bool empty() const;
  };

  template < class Key, class Value, class Hash1, class Hash2, class Equal >
  size_t CuckooHash< Key, Value, Hash1, Hash2, Equal >::hashPos1(const Key &key) const
  {
    return hash1_(key) % capacity_;
  }

  template < class Key, class Value, class Hash1, class Hash2, class Equal >
  size_t CuckooHash< Key, Value, Hash1, Hash2, Equal >::hashPos2(const Key &key) const
  {
    return hash2_(key) % capacity_;
  }

  template < class Key, class Value, class Hash1, class Hash2, class Equal >
  void CuckooHash< Key, Value, Hash1, Hash2, Equal >::swap(CuckooHash &other) noexcept
  {
    std::swap(table1_, other.table1_);
    std::swap(table2_, other.table2_);
    std::swap(capacity_, other.capacity_);
    std::swap(size_, other.size_);
    std::swap(hash1_, other.hash1_);
    std::swap(hash2_, other.hash2_);
    std::swap(equal_, other.equal_);
  }

  template < class Key, class Value, class Hash1, class Hash2, class Equal >
  void CuckooHash< Key, Value, Hash1, Hash2, Equal >::insertWithoutCheck(const Key &key, const Value &value)
  {
    std::pair< Key, Value > current(key, value);
    bool firstTable = true;
    size_t kickCount = 0;
    while (kickCount < capacity_) {
      if (firstTable) {
        size_t pos = hashPos1(current.first);
        if (!table1_[pos].occupied) {
          table1_[pos].data = current;
          table1_[pos].occupied = true;
          ++size_;
          return;
        }
        std::swap(current, table1_[pos].data);
        firstTable = false;
      } else {
        size_t pos = hashPos2(current.first);
        if (!table2_[pos].occupied) {
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

  template < class Key, class Value, class Hash1, class Hash2, class Equal >
  CuckooHash< Key, Value, Hash1, Hash2, Equal >::CuckooHash(size_t capacity):
    table1_(new Item[capacity]),
    table2_(new Item[capacity]),
    capacity_(capacity),
    size_(0)
  {}

  template < class Key, class Value, class Hash1, class Hash2, class Equal >
  CuckooHash< Key, Value, Hash1, Hash2, Equal >::CuckooHash(const CuckooHash &other):
    table1_(new Item[other.capacity_]),
    table2_(new Item[other.capacity_]),
    capacity_(other.capacity_),
    size_(other.size_),
    hash1_(other.hash1_),
    hash2_(other.hash2_),
    equal_(other.equal_)
  {
    for (size_t i = 0; i < capacity_; ++i) {
      table1_[i] = other.table1_[i];
      table2_[i] = other.table2_[i];
    }
  }

  template < class Key, class Value, class Hash1, class Hash2, class Equal >
  CuckooHash< Key, Value, Hash1, Hash2, Equal >::CuckooHash(CuckooHash &&other) noexcept:
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

  template < class Key, class Value, class Hash1, class Hash2, class Equal >
  CuckooHash< Key, Value, Hash1, Hash2, Equal >::~CuckooHash()
  {
    delete[] table1_;
    delete[] table2_;
  }

  template < class Key, class Value, class Hash1, class Hash2, class Equal >
  CuckooHash< Key, Value, Hash1, Hash2, Equal > &
  CuckooHash< Key, Value, Hash1, Hash2, Equal >::operator=(const CuckooHash &other)
  {
    if (this != &other) {
      CuckooHash temp(other);
      swap(temp);
    }
    return *this;
  }

  template < class Key, class Value, class Hash1, class Hash2, class Equal >
  CuckooHash< Key, Value, Hash1, Hash2, Equal > &
  CuckooHash< Key, Value, Hash1, Hash2, Equal >::operator=(CuckooHash &&other) noexcept
  {
    if (this != &other) {
      swap(other);
    }
    return *this;
  }

  template < class Key, class Value, class Hash1, class Hash2, class Equal >
  void CuckooHash< Key, Value, Hash1, Hash2, Equal >::insert(const Key &key, const Value &value)
  {
    if (has(key)) {
      throw std::logic_error("Key already exists");
    }
    insertWithoutCheck(key, value);
  }

  template < class Key, class Value, class Hash1, class Hash2, class Equal >
  Value CuckooHash< Key, Value, Hash1, Hash2, Equal >::get(const Key &key) const
  {
    size_t pos1 = hashPos1(key);
    if (table1_[pos1].occupied && equal_(table1_[pos1].data.first, key)) {
      return table1_[pos1].data.second;
    }
    size_t pos2 = hashPos2(key);
    if (table2_[pos2].occupied && equal_(table2_[pos2].data.first, key)) {
      return table2_[pos2].data.second;
    }
    throw std::out_of_range("Key does not exist");
  }

  template < class Key, class Value, class Hash1, class Hash2, class Equal >
  bool CuckooHash< Key, Value, Hash1, Hash2, Equal >::has(const Key &key) const
  {
    size_t pos1 = hashPos1(key);
    if (table1_[pos1].occupied && equal_(table1_[pos1].data.first, key)) {
      return true;
    }
    size_t pos2 = hashPos2(key);
    if (table2_[pos2].occupied && equal_(table2_[pos2].data.first, key)) {
      return true;
    }
    return false;
  }

  template < class Key, class Value, class Hash1, class Hash2, class Equal >
  bool CuckooHash< Key, Value, Hash1, Hash2, Equal >::remove(const Key &key)
  {
    size_t pos1 = hashPos1(key);
    if (table1_[pos1].occupied && equal_(table1_[pos1].data.first, key)) {
      table1_[pos1].occupied = false;
      --size_;
      return true;
    }
    size_t pos2 = hashPos2(key);
    if (table2_[pos2].occupied && equal_(table2_[pos2].data.first, key)) {
      table2_[pos2].occupied = false;
      --size_;
      return true;
    }
    return false;
  }

  template < class Key, class Value, class Hash1, class Hash2, class Equal >
  void CuckooHash< Key, Value, Hash1, Hash2, Equal >::clear()
  {
    Item *newTable1 = nullptr;
    Item *newTable2 = nullptr;
    try {
      newTable1 = new Item[capacity_];
      newTable2 = new Item[capacity_];
    } catch (...) {
      delete[] newTable1;
      delete[] newTable2;
      throw;
    }
    delete[] table1_;
    delete[] table2_;
    table1_ = newTable1;
    table2_ = newTable2;
    size_ = 0;
  }

  template < class Key, class Value, class Hash1, class Hash2, class Equal >
  void CuckooHash< Key, Value, Hash1, Hash2, Equal >::rehash(size_t newCapacity)
  {
    Item *oldTable1 = table1_;
    Item *oldTable2 = table2_;
    size_t oldCapacity = capacity_;
    size_t oldSize = size_;
    Item *newTable1 = new Item[newCapacity];
    Item *newTable2 = nullptr;
    try {
      newTable2 = new Item[newCapacity];
    } catch (...) {
      delete[] newTable1;
      throw;
    }
    table1_ = newTable1;
    table2_ = newTable2;
    capacity_ = newCapacity;
    size_ = 0;
    try {
      for (size_t i = 0; i < oldCapacity; ++i) {
        if (oldTable1[i].occupied) {
          insertWithoutCheck(oldTable1[i].data.first, oldTable1[i].data.second);
        }
      }
      for (size_t i = 0; i < oldCapacity; ++i) {
        if (oldTable2[i].occupied) {
          insertWithoutCheck(oldTable2[i].data.first, oldTable2[i].data.second);
        }
      }
    } catch (...) {
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

  template < class Key, class Value, class Hash1, class Hash2, class Equal >
  size_t CuckooHash< Key, Value, Hash1, Hash2, Equal >::size() const
  {
    return size_;
  }

  template < class Key, class Value, class Hash1, class Hash2, class Equal >
  bool CuckooHash< Key, Value, Hash1, Hash2, Equal >::empty() const
  {
    return size_ == 0;
  }
}

#endif
