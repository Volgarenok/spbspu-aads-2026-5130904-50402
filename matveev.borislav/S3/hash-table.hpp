#ifndef MATVEEV_HASH_TABLE_HPP
#define MATVEEV_HASH_TABLE_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <limits>

namespace matveev
{
template< class Key, class Value >
struct HashTableItem
{
  HashTableItem();
  Key key;
  Value value;
  bool occupied;
};

template< class Key, class Value, class Hash, class Equal >
class HashTable
{
public:
  HashTable(size_t buckets, size_t bucket_capacity);
  HashTable(const HashTable& other);
  HashTable& operator=(const HashTable& other);
  ~HashTable();

  void add(const Key& key, const Value& value);
  bool has(const Key& key) const;
  Value& at(const Key& key);
  const Value& at(const Key& key) const;
  Value drop(const Key& key);
  void clear() noexcept;
  void swap(HashTable& other) noexcept;
  void rehash(size_t buckets, size_t bucket_capacity);

  size_t size() const noexcept;
  size_t bucketCount() const noexcept;
  size_t bucketCapacity() const noexcept;

private:
  HashTableItem< Key, Value >* data_;
  size_t bucket_count_;
  size_t bucket_capacity_;
  size_t size_;
  Hash hash_;
  Equal equal_;

  size_t indexOf(const Key& key) const;
  size_t capacity() const noexcept;
};

template< class Key, class Value >
HashTableItem< Key, Value >::HashTableItem():
  key(),
  value(),
  occupied(false)
{}

template< class Key, class Value, class Hash, class Equal >
HashTable< Key, Value, Hash, Equal >::HashTable(size_t buckets, size_t bucket_capacity):
  data_(nullptr),
  bucket_count_(buckets),
  bucket_capacity_(bucket_capacity),
  size_(0),
  hash_(),
  equal_()
{
  if (bucket_count_ == 0 || bucket_capacity_ == 0)
  {
    throw std::invalid_argument("invalid hash table capacity");
  }

  if (bucket_count_ > std::numeric_limits< size_t >::max() / bucket_capacity_)
  {
    throw std::overflow_error("hash table capacity overflow");
  }

  data_ = new HashTableItem< Key, Value >[capacity()];
}

template< class Key, class Value, class Hash, class Equal >
HashTable< Key, Value, Hash, Equal >::~HashTable()
{
  delete[] data_;
}

template< class Key, class Value, class Hash, class Equal >
size_t HashTable< Key, Value, Hash, Equal >::size() const noexcept
{
  return size_;
}

template< class Key, class Value, class Hash, class Equal >
size_t HashTable< Key, Value, Hash, Equal >::bucketCount() const noexcept
{
  return bucket_count_;
}

template< class Key, class Value, class Hash, class Equal >
size_t HashTable< Key, Value, Hash, Equal >::bucketCapacity() const noexcept
{
  return bucket_capacity_;
}

template< class Key, class Value, class Hash, class Equal >
size_t HashTable< Key, Value, Hash, Equal >::indexOf(const Key& key) const
{
  return hash_(key) % bucket_count_;
}

template< class Key, class Value, class Hash, class Equal >
size_t HashTable< Key, Value, Hash, Equal >::capacity() const noexcept
{
  return bucket_count_ * bucket_capacity_;
}

}

#endif
