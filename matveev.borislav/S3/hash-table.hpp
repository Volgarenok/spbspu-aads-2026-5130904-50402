#ifndef MATVEEV_HASH_TABLE_HPP
#define MATVEEV_HASH_TABLE_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>

namespace matveev
{
template< class Key, class Value >
struct HashTableItem
{
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
};

}

#endif
