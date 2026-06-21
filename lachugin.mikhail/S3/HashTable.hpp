#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP
#include "../common/list.hpp"
#include "../common/itters.hpp"
#include "../common/node.hpp"
#include <stdexcept>
#include "HashItem.hpp"
#include "HashIter.hpp"
namespace lachugin
{
  template< class Key, class Value, class Hash, class Equal >
  class HashIter;

  template< class Key, class Value, class Hash, class Equal >
  class HashConstIter;

  template< class Key, class Value, class Hash, class Equal >
  class HashTable
  {
    using value_type = std::pair< Key, Value >;
    using it = HashIter< Key, Value, Hash, Equal >;
    using ConstIt = HashConstIter< Key, Value, Hash, Equal >;
  public:
    friend class HashIter< Key, Value, Hash, Equal >;
    friend class HashConstIter< Key, Value, Hash, Equal >;
    HashTable(size_t bucketCount, size_t bucketCapacity, size_t spareCapacity = 10);
    ~HashTable();
    HashTable(const HashTable& other);
    HashTable& operator=(const HashTable& other);
    HashTable(HashTable&& other) noexcept;
    HashTable& operator=(HashTable&& other) noexcept;

    it begin();
    it end();
    ConstIt begin() const;
    ConstIt end() const;

    void add(const Key& k, const Value& v);
    Value drop(const Key& k);
    bool has(const Key& k) const;
    Value& get(const Key& k);
    const Value& get(const Key& k) const;

    size_t size() const noexcept;
    bool empty() const noexcept;

    void clear();
    void swap(HashTable& other) noexcept;

    size_t bucketFirst(size_t bucket) const noexcept;
    size_t overflowFirst() const noexcept;

    void rehash(size_t newBucketCount);
  private:
    HashItem< Key, Value >* data_;
    size_t size_;

    size_t bucketCount_;
    size_t bucketCapacity_;
    size_t spareCapacity_;

    Hash hasher_;
    Equal equal_;
  };

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(size_t bucketCount, size_t bucketCapacity,
    size_t spareCapacity):
  data_(nullptr),
  size_(0),
  bucketCount_(bucketCount),
  bucketCapacity_(bucketCapacity),
  spareCapacity_(spareCapacity),
  hasher_(Hash()),
  equal_(Equal())
  {
    if (bucketCount == 0 || bucketCapacity == 0)
    {
      throw std::invalid_argument("Invalid table size");
    }

    data_ = new HashItem< Key, Value >[bucketCount_ * bucketCapacity_ + spareCapacity_];
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::bucketFirst(size_t bucket) const noexcept
  {
    return bucket * bucketCapacity_;
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::overflowFirst() const noexcept
  {
    return bucketCount_ * bucketCapacity_;
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::~HashTable()
  {
    delete[] data_;
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::add(const Key& key, const Value& value)
  {
    if (has(key))
    {
      throw std::logic_error("Key already exists");
    }

    size_t bucket = hasher_(key) % bucketCount_;
    size_t first = bucketFirst(bucket);
    for (size_t i = 0; i < bucketCapacity_; ++i)
    {
      size_t pos = first + i;

      if (!data_[pos].occupied)
      {
        data_[pos].key = key;
        data_[pos].value = value;
        data_[pos].occupied = true;

        ++size_;
        return;
      }
    }
    size_t overflow = overflowFirst();
    for (size_t i = 0; i < spareCapacity_; ++i)
    {
      size_t pos = overflow + i;

      if (!data_[pos].occupied)
      {
        data_[pos].key = key;
        data_[pos].value = value;
        data_[pos].occupied = true;
        ++size_;
        return;
      }
    }
    rehash(bucketCount_ * 2);
    add(key, value);
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HashTable< Key, Value, Hash, Equal >::has(const Key& key) const
  {
    size_t bucket = hasher_(key) % bucketCount_;
    size_t first = bucketFirst(bucket);
    for (size_t i = 0; i < bucketCapacity_; ++i)
    {
      size_t pos = first + i;
      if (data_[pos].occupied && equal_(data_[pos].key, key))
      {
        return true;
      }
    }
    size_t overflow = overflowFirst();
    for (size_t i = 0; i < spareCapacity_; ++i)
    {
      size_t pos = overflow + i;
      if (data_[pos].occupied && equal_(data_[pos].key, key))
      {
        return true;
      }
    }
    return false;
  }

  template< class Key, class Value, class Hash, class Equal >
  Value HashTable< Key, Value, Hash, Equal >::drop(const Key& key)
  {
    size_t bucket = hasher_(key) % bucketCount_;
    size_t first = bucketFirst(bucket);
    for (size_t i = 0; i < bucketCapacity_; ++i)
    {
      size_t pos = first + i;
      if (data_[pos].occupied && equal_(data_[pos].key, key))
      {
        Value result = data_[pos].value;
        data_[pos].occupied = false;
        --size_;
        return result;
      }
    }
    size_t overflow = overflowFirst();
    for (size_t i = 0; i < spareCapacity_; ++i)
    {
      size_t pos = overflow + i;
      if (data_[pos].occupied && equal_(data_[pos].key, key))
      {
        Value result = data_[pos].value;
        data_[pos].occupied = false;
        --size_;
        return result;
      }
    }
    throw std::out_of_range("Key not found");
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::size() const noexcept
  {
    return size_;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HashTable< Key, Value, Hash, Equal >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< class Key, class Value, class Hash, class Equal >
  Value& HashTable< Key, Value, Hash, Equal >::get(const Key& key)
  {
    size_t bucket = hasher_(key) % bucketCount_;
    size_t first = bucketFirst(bucket);
    for (size_t i = 0; i < bucketCapacity_; ++i)
    {
      size_t pos = first + i;

      if (data_[pos].occupied && equal_(data_[pos].key, key))
      {
        return data_[pos].value;
      }
    }
    size_t overflow = overflowFirst();

    for (size_t i = 0; i < spareCapacity_; ++i)
    {
      size_t pos = overflow + i;
      if (data_[pos].occupied && equal_(data_[pos].key, key))
      {
        return data_[pos].value;
      }
    }

    throw std::out_of_range("Key not found");
  }

  template< class Key, class Value, class Hash, class Equal >
  const Value& HashTable< Key, Value, Hash, Equal >::get(const Key& key) const
  {
    size_t bucket = hasher_(key) % bucketCount_;
    size_t first = bucketFirst(bucket);
    for (size_t i = 0; i < bucketCapacity_; ++i)
    {
      size_t pos = first + i;
      if (data_[pos].occupied && equal_(data_[pos].key, key))
      {
        return data_[pos].value;
      }
    }
    size_t overflow = overflowFirst();
    for (size_t i = 0; i < spareCapacity_; ++i)
    {
      size_t pos = overflow + i;
      if (data_[pos].occupied && equal_(data_[pos].key, key))
      {
        return data_[pos].value;
      }
    }
    throw std::out_of_range("Key not found");
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::clear()
  {
    size_t cap = bucketCount_ * bucketCapacity_ + spareCapacity_;
    for (size_t i = 0; i < cap; ++i)
    {
      data_[i].occupied = false;
    }
    size_ = 0;
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(const HashTable& other):
  data_(nullptr),
  size_(other.size_),
  bucketCount_(other.bucketCount_),
  bucketCapacity_(other.bucketCapacity_),
  spareCapacity_(other.spareCapacity_),
  hasher_(other.hasher_),
  equal_(other.equal_)
  {
    size_t cap = bucketCount_ * bucketCapacity_ + spareCapacity_;
    data_ = new HashItem< Key, Value >[cap];
    for (size_t i = 0; i < cap; ++i)
    {
      data_[i] = other.data_[i];
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(HashTable&& other) noexcept:
  data_(other.data_),
  size_(other.size_),
  bucketCount_(other.bucketCount_),
  bucketCapacity_(other.bucketCapacity_),
  spareCapacity_(other.spareCapacity_),
  hasher_(std::move(other.hasher_)),
  equal_(std::move(other.equal_))
  {
    other.data_ = nullptr;
    other.bucketCount_ = 0;
    other.bucketCapacity_ = 0;
    other.spareCapacity_ = 0;
    other.size_ = 0;
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::swap(HashTable& other) noexcept
  {
    std::swap(data_, other.data_);
    std::swap(bucketCount_, other.bucketCount_);
    std::swap(bucketCapacity_, other.bucketCapacity_);
    std::swap(spareCapacity_, other.spareCapacity_);
    std::swap(size_, other.size_);
    std::swap(hasher_, other.hasher_);
    std::swap(equal_, other.equal_);
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >&
    HashTable< Key, Value, Hash, Equal >::operator=(const HashTable& other)
  {
    if (this != &other)
    {
      HashTable tmp(other);
      swap(tmp);
    }
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable<Key, Value, Hash, Equal>& HashTable<Key, Value, Hash, Equal>::operator=(
  HashTable&& other) noexcept
  {
    if (this != &other)
    {
      delete[] data_;
      data_ = other.data_;
      size_ = other.size_;
      bucketCount_ = other.bucketCount_;
      bucketCapacity_ = other.bucketCapacity_;
      spareCapacity_ = other.spareCapacity_;
      other.data_ = nullptr;
      other.size_ = 0;
      other.bucketCount_ = 0;
      other.bucketCapacity_ = 0;
      other.spareCapacity_ = 0;
    }
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  HashIter< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::begin()
  {
    return HashIter< Key, Value, Hash, Equal >(this, 0);
  }

  template< class Key, class Value, class Hash, class Equal >
  HashIter< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::end()
  {
    return HashIter< Key, Value, Hash, Equal >(this,bucketCount_ * bucketCapacity_ + spareCapacity_);
  }

  template< class Key, class Value, class Hash, class Equal >
  HashConstIter< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::begin() const
  {
    return HashConstIter< Key, Value, Hash, Equal >(this, 0);
  }

  template< class Key, class Value, class Hash, class Equal >
  HashConstIter< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::end() const
  {
    return HashConstIter< Key, Value, Hash, Equal >(this,bucketCount_ * bucketCapacity_ + spareCapacity_);
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::rehash(size_t newBucketCount)
  {
    HashTable tmp(newBucketCount, bucketCapacity_, spareCapacity_ * 2);
    for (auto it = begin(); it != end(); ++it)
    {
      tmp.add(it->key, it->value);
    }
    swap(tmp);
  }
}
#endif
