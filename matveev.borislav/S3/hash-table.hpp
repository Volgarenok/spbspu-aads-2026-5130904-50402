#ifndef MATVEEV_HASH_TABLE_HPP
#define MATVEEV_HASH_TABLE_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <limits>
#include <memory>

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
  class Iterator;
  class ConstIterator;

  using iterator = Iterator;
  using const_iterator = ConstIterator;
  using SizeUpdateFunction = size_t (*)(size_t);

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

  Iterator begin() noexcept;
  Iterator end() noexcept;
  ConstIterator begin() const noexcept;
  ConstIterator end() const noexcept;
  ConstIterator cbegin() const noexcept;
  ConstIterator cend() const noexcept;

  size_t size() const noexcept;
  size_t bucketCount() const noexcept;
  size_t bucketCapacity() const noexcept;
  double loadFactor() const noexcept;
  size_t spareBucketSize() const noexcept;
  double averageBucketSize() const noexcept;
  size_t maxBucketSize() const noexcept;

  void maxLoadFactor(double value);
  void maxSpareBucketSize(size_t value) noexcept;
  void maxAverageBucketSize(double value);
  void bucketUpdateFunction(SizeUpdateFunction function);
  void bucketSizeUpdateFunction(SizeUpdateFunction function);

private:
  HashTableItem< Key, Value >* data_;
  size_t bucket_count_;
  size_t bucket_capacity_;
  size_t size_;
  double max_load_factor_;
  size_t max_spare_bucket_size_;
  double max_average_bucket_size_;
  SizeUpdateFunction bucket_update_function_;
  SizeUpdateFunction bucket_size_update_function_;
  Hash hash_;
  Equal equal_;

  void addRaw(const Key& key, const Value& value);
  bool needRehash() const noexcept;
  void rehashByLimits();
  size_t indexOf(const Key& key) const;
  size_t capacity() const noexcept;
  size_t bucketFirst(size_t bucket) const noexcept;
  size_t overflowFirst() const noexcept;
  static size_t defaultSizeUpdate(size_t value);
};

template< class Key, class Value, class Hash, class Equal >
class HashTable< Key, Value, Hash, Equal >::Iterator
{
  friend class HashTable< Key, Value, Hash, Equal >;

public:
  Iterator();

  HashTableItem< Key, Value >& operator*() const;
  HashTableItem< Key, Value >* operator->() const;

  Iterator& operator++();
  Iterator operator++(int);

  bool operator==(const Iterator& other) const;
  bool operator!=(const Iterator& other) const;

private:
  Iterator(HashTable< Key, Value, Hash, Equal >* table, size_t pos);
  void skipEmpty() noexcept;

  HashTable< Key, Value, Hash, Equal >* table_;
  size_t pos_;
};

template< class Key, class Value, class Hash, class Equal >
HashTable< Key, Value, Hash, Equal >::Iterator::Iterator():
  table_(nullptr),
  pos_(0)
{}

template< class Key, class Value, class Hash, class Equal >
HashTable< Key, Value, Hash, Equal >::Iterator::Iterator(HashTable< Key, Value, Hash, Equal >* table, size_t pos):
  table_(table),
  pos_(pos)
{
  skipEmpty();
}

template< class Key, class Value, class Hash, class Equal >
HashTableItem< Key, Value >& HashTable< Key, Value, Hash, Equal >::Iterator::operator*() const
{
  return table_->data_[pos_];
}

template< class Key, class Value, class Hash, class Equal >
HashTableItem< Key, Value >* HashTable< Key, Value, Hash, Equal >::Iterator::operator->() const
{
  return std::addressof(table_->data_[pos_]);
}

template< class Key, class Value, class Hash, class Equal >
typename HashTable< Key, Value, Hash, Equal >::Iterator& HashTable< Key, Value, Hash, Equal >::Iterator::operator++()
{
  if (table_ != nullptr && pos_ < table_->capacity())
  {
    ++pos_;
    skipEmpty();
  }

  return *this;
}

template< class Key, class Value, class Hash, class Equal >
typename HashTable< Key, Value, Hash, Equal >::Iterator HashTable< Key, Value, Hash, Equal >::Iterator::operator++(int)
{
  Iterator result(*this);
  ++(*this);
  return result;
}

template< class Key, class Value, class Hash, class Equal >
bool HashTable< Key, Value, Hash, Equal >::Iterator::operator==(const Iterator& other) const
{
  return table_ == other.table_ && pos_ == other.pos_;
}

template< class Key, class Value, class Hash, class Equal >
bool HashTable< Key, Value, Hash, Equal >::Iterator::operator!=(const Iterator& other) const
{
  return !(*this == other);
}

template< class Key, class Value, class Hash, class Equal >
void HashTable< Key, Value, Hash, Equal >::Iterator::skipEmpty() noexcept
{
  while (table_ != nullptr && pos_ < table_->capacity() && !table_->data_[pos_].occupied)
  {
    ++pos_;
  }
}

template< class Key, class Value, class Hash, class Equal >
class HashTable< Key, Value, Hash, Equal >::ConstIterator
{
  friend class HashTable< Key, Value, Hash, Equal >;

public:
  ConstIterator();

  const HashTableItem< Key, Value >& operator*() const;
  const HashTableItem< Key, Value >* operator->() const;

  ConstIterator& operator++();
  ConstIterator operator++(int);

  bool operator==(const ConstIterator& other) const;
  bool operator!=(const ConstIterator& other) const;

private:
  ConstIterator(const HashTable< Key, Value, Hash, Equal >* table, size_t pos);
  void skipEmpty() noexcept;

  const HashTable< Key, Value, Hash, Equal >* table_;
  size_t pos_;
};

template< class Key, class Value, class Hash, class Equal >
HashTable< Key, Value, Hash, Equal >::ConstIterator::ConstIterator():
  table_(nullptr),
  pos_(0)
{}

template< class Key, class Value, class Hash, class Equal >
HashTable< Key, Value, Hash, Equal >::ConstIterator::ConstIterator(
  const HashTable< Key, Value, Hash, Equal >* table,
  size_t pos
):
  table_(table),
  pos_(pos)
{
  skipEmpty();
}

template< class Key, class Value, class Hash, class Equal >
const HashTableItem< Key, Value >& HashTable< Key, Value, Hash, Equal >::ConstIterator::operator*() const
{
  return table_->data_[pos_];
}

template< class Key, class Value, class Hash, class Equal >
const HashTableItem< Key, Value >* HashTable< Key, Value, Hash, Equal >::ConstIterator::operator->() const
{
  return std::addressof(table_->data_[pos_]);
}

template< class Key, class Value, class Hash, class Equal >
typename HashTable< Key, Value, Hash, Equal >::ConstIterator& HashTable< Key, Value, Hash, Equal >::ConstIterator::operator++()
{
  if (table_ != nullptr && pos_ < table_->capacity())
  {
    ++pos_;
    skipEmpty();
  }

  return *this;
}

template< class Key, class Value, class Hash, class Equal >
typename HashTable< Key, Value, Hash, Equal >::ConstIterator HashTable< Key, Value, Hash, Equal >::ConstIterator::operator++(int)
{
  ConstIterator result(*this);
  ++(*this);
  return result;
}

template< class Key, class Value, class Hash, class Equal >
bool HashTable< Key, Value, Hash, Equal >::ConstIterator::operator==(const ConstIterator& other) const
{
  return table_ == other.table_ && pos_ == other.pos_;
}

template< class Key, class Value, class Hash, class Equal >
bool HashTable< Key, Value, Hash, Equal >::ConstIterator::operator!=(const ConstIterator& other) const
{
  return !(*this == other);
}

template< class Key, class Value, class Hash, class Equal >
void HashTable< Key, Value, Hash, Equal >::ConstIterator::skipEmpty() noexcept
{
  while (table_ != nullptr && pos_ < table_->capacity() && !table_->data_[pos_].occupied)
  {
    ++pos_;
  }
}

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
  max_load_factor_(1.0),
  max_spare_bucket_size_(bucket_capacity),
  max_average_bucket_size_(static_cast< double >(bucket_capacity)),
  bucket_update_function_(defaultSizeUpdate),
  bucket_size_update_function_(defaultSizeUpdate),
  hash_(),
  equal_()
{
  if (bucket_count_ == 0 || bucket_capacity_ == 0)
  {
    throw std::invalid_argument("invalid hash table capacity");
  }

  if (bucket_count_ == std::numeric_limits< size_t >::max())
  {
    throw std::overflow_error("hash table capacity overflow");
  }

  if (bucket_count_ + 1 > std::numeric_limits< size_t >::max() / bucket_capacity_)
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
HashTable< Key, Value, Hash, Equal >::HashTable(const HashTable& other):
  data_(nullptr),
  bucket_count_(other.bucket_count_),
  bucket_capacity_(other.bucket_capacity_),
  size_(0),
  max_load_factor_(other.max_load_factor_),
  max_spare_bucket_size_(other.max_spare_bucket_size_),
  max_average_bucket_size_(other.max_average_bucket_size_),
  bucket_update_function_(other.bucket_update_function_),
  bucket_size_update_function_(other.bucket_size_update_function_),
  hash_(other.hash_),
  equal_(other.equal_)
{
  data_ = new HashTableItem< Key, Value >[capacity()];

  try
  {
    for (size_t i = 0; i < other.capacity(); ++i)
    {
      if (other.data_[i].occupied)
      {
        addRaw(other.data_[i].key, other.data_[i].value);
      }
    }
  }
  catch (...)
  {
    delete[] data_;
    data_ = nullptr;
    throw;
  }
}

template< class Key, class Value, class Hash, class Equal >
HashTable< Key, Value, Hash, Equal >& HashTable< Key, Value, Hash, Equal >::operator=(const HashTable& other)
{
  if (this == &other)
  {
    return *this;
  }

  HashTable< Key, Value, Hash, Equal > temp(other);
  swap(temp);

  return *this;
}

template< class Key, class Value, class Hash, class Equal >
void HashTable< Key, Value, Hash, Equal >::add(const Key& key, const Value& value)
{
  addRaw(key, value);

  if (needRehash())
  {
    rehashByLimits();
  }
}

template< class Key, class Value, class Hash, class Equal >
void HashTable< Key, Value, Hash, Equal >::addRaw(const Key& key, const Value& value)
{
  size_t bucket = indexOf(key);
  size_t first = bucketFirst(bucket);
  size_t overflow = overflowFirst();

  for (size_t i = 0; i < bucket_capacity_; ++i)
  {
    HashTableItem< Key, Value >& item = data_[first + i];

    if (item.occupied && equal_(item.key, key))
    {
      throw std::logic_error("key already exists");
    }
  }

  for (size_t i = 0; i < bucket_capacity_; ++i)
  {
    HashTableItem< Key, Value >& item = data_[overflow + i];

    if (item.occupied && equal_(item.key, key))
    {
      throw std::logic_error("key already exists");
    }
  }

  for (size_t i = 0; i < bucket_capacity_; ++i)
  {
    HashTableItem< Key, Value >& item = data_[first + i];

    if (!item.occupied)
    {
      item.key = key;
      item.value = value;
      item.occupied = true;
      ++size_;
      return;
    }
  }

  for (size_t i = 0; i < bucket_capacity_; ++i)
  {
    HashTableItem< Key, Value >& item = data_[overflow + i];

    if (!item.occupied)
    {
      item.key = key;
      item.value = value;
      item.occupied = true;
      ++size_;
      return;
    }
  }

  throw std::overflow_error("hash table overflow bucket overflow");
}

template< class Key, class Value, class Hash, class Equal >
bool HashTable< Key, Value, Hash, Equal >::needRehash() const noexcept
{
  const bool load_limit = loadFactor() > max_load_factor_;
  const bool spare_limit = spareBucketSize() > max_spare_bucket_size_;
  const bool average_limit = averageBucketSize() > max_average_bucket_size_;

  return load_limit || spare_limit || average_limit;
}

template< class Key, class Value, class Hash, class Equal >
void HashTable< Key, Value, Hash, Equal >::rehashByLimits()
{
  size_t new_bucket_count = bucket_count_;
  size_t new_bucket_capacity = bucket_capacity_;

  bool load_limit = true;
  bool average_limit = true;

  while (load_limit || average_limit)
  {
    const double new_load = static_cast< double >(size_) / static_cast< double >(new_bucket_count * new_bucket_capacity);
    const double new_average = static_cast< double >(size_) / static_cast< double >(new_bucket_count);

    load_limit = new_load > max_load_factor_;
    average_limit = new_average > max_average_bucket_size_;

    if (load_limit || average_limit)
    {
      new_bucket_count = bucket_update_function_(new_bucket_count);
    }
  }

  if (spareBucketSize() > max_spare_bucket_size_)
  {
    new_bucket_capacity = bucket_size_update_function_(new_bucket_capacity);
  }

  if (new_bucket_count == bucket_count_ && new_bucket_capacity == bucket_capacity_)
  {
    new_bucket_count = bucket_update_function_(new_bucket_count);
  }

  rehash(new_bucket_count, new_bucket_capacity);
}

template< class Key, class Value, class Hash, class Equal >
bool HashTable< Key, Value, Hash, Equal >::has(const Key& key) const
{
  size_t bucket = indexOf(key);
  size_t first = bucketFirst(bucket);
  size_t overflow = overflowFirst();

  for (size_t i = 0; i < bucket_capacity_; ++i)
  {
    const HashTableItem< Key, Value >& item = data_[first + i];

    if (item.occupied && equal_(item.key, key))
    {
      return true;
    }
  }

  for (size_t i = 0; i < bucket_capacity_; ++i)
  {
    const HashTableItem< Key, Value >& item = data_[overflow + i];

    if (item.occupied && equal_(item.key, key))
    {
      return true;
    }
  }

  return false;
}

template< class Key, class Value, class Hash, class Equal >
Value& HashTable< Key, Value, Hash, Equal >::at(const Key& key)
{
  size_t bucket = indexOf(key);
  size_t first = bucketFirst(bucket);
  size_t overflow = overflowFirst();

  for (size_t i = 0; i < bucket_capacity_; ++i)
  {
    HashTableItem< Key, Value >& item = data_[first + i];

    if (item.occupied && equal_(item.key, key))
    {
      return item.value;
    }
  }

  for (size_t i = 0; i < bucket_capacity_; ++i)
  {
    HashTableItem< Key, Value >& item = data_[overflow + i];

    if (item.occupied && equal_(item.key, key))
    {
      return item.value;
    }
  }

  throw std::out_of_range("key not found");
}

template< class Key, class Value, class Hash, class Equal >
const Value& HashTable< Key, Value, Hash, Equal >::at(const Key& key) const
{
  size_t bucket = indexOf(key);
  size_t first = bucketFirst(bucket);
  size_t overflow = overflowFirst();

  for (size_t i = 0; i < bucket_capacity_; ++i)
  {
    const HashTableItem< Key, Value >& item = data_[first + i];

    if (item.occupied && equal_(item.key, key))
    {
      return item.value;
    }
  }

  for (size_t i = 0; i < bucket_capacity_; ++i)
  {
    const HashTableItem< Key, Value >& item = data_[overflow + i];

    if (item.occupied && equal_(item.key, key))
    {
      return item.value;
    }
  }

  throw std::out_of_range("key not found");
}

template< class Key, class Value, class Hash, class Equal >
Value HashTable< Key, Value, Hash, Equal >::drop(const Key& key)
{
  size_t bucket = indexOf(key);
  size_t first = bucketFirst(bucket);
  size_t overflow = overflowFirst();

  for (size_t i = 0; i < bucket_capacity_; ++i)
  {
    HashTableItem< Key, Value >& item = data_[first + i];

    if (item.occupied && equal_(item.key, key))
    {
      Value value = item.value;
      item.occupied = false;
      --size_;
      return value;
    }
  }

  for (size_t i = 0; i < bucket_capacity_; ++i)
  {
    HashTableItem< Key, Value >& item = data_[overflow + i];

    if (item.occupied && equal_(item.key, key))
    {
      Value value = item.value;
      item.occupied = false;
      --size_;
      return value;
    }
  }

  throw std::out_of_range("key not found");
}

template< class Key, class Value, class Hash, class Equal >
void HashTable< Key, Value, Hash, Equal >::clear() noexcept
{
  for (size_t i = 0; i < capacity(); ++i)
  {
    data_[i].occupied = false;
  }

  size_ = 0;
}

template< class Key, class Value, class Hash, class Equal >
void HashTable< Key, Value, Hash, Equal >::swap(HashTable& other) noexcept
{
  using std::swap;

  swap(data_, other.data_);
  swap(bucket_count_, other.bucket_count_);
  swap(bucket_capacity_, other.bucket_capacity_);
  swap(size_, other.size_);
  swap(max_load_factor_, other.max_load_factor_);
  swap(max_spare_bucket_size_, other.max_spare_bucket_size_);
  swap(max_average_bucket_size_, other.max_average_bucket_size_);
  swap(bucket_update_function_, other.bucket_update_function_);
  swap(bucket_size_update_function_, other.bucket_size_update_function_);
  swap(hash_, other.hash_);
  swap(equal_, other.equal_);
}

template< class Key, class Value, class Hash, class Equal >
void HashTable< Key, Value, Hash, Equal >::rehash(size_t buckets, size_t bucket_capacity)
{
  HashTable< Key, Value, Hash, Equal > temp(buckets, bucket_capacity);
  temp.max_load_factor_ = max_load_factor_;
  temp.max_spare_bucket_size_ = max_spare_bucket_size_;
  temp.max_average_bucket_size_ = max_average_bucket_size_;
  temp.bucket_update_function_ = bucket_update_function_;
  temp.bucket_size_update_function_ = bucket_size_update_function_;

  for (size_t i = 0; i < capacity(); ++i)
  {
    if (data_[i].occupied)
    {
      temp.addRaw(data_[i].key, data_[i].value);
    }
  }

  swap(temp);
}

template< class Key, class Value, class Hash, class Equal >
typename HashTable< Key, Value, Hash, Equal >::Iterator HashTable< Key, Value, Hash, Equal >::begin() noexcept
{
  return Iterator(this, 0);
}

template< class Key, class Value, class Hash, class Equal >
typename HashTable< Key, Value, Hash, Equal >::Iterator HashTable< Key, Value, Hash, Equal >::end() noexcept
{
  return Iterator(this, capacity());
}

template< class Key, class Value, class Hash, class Equal >
typename HashTable< Key, Value, Hash, Equal >::ConstIterator HashTable< Key, Value, Hash, Equal >::begin() const noexcept
{
  return ConstIterator(this, 0);
}

template< class Key, class Value, class Hash, class Equal >
typename HashTable< Key, Value, Hash, Equal >::ConstIterator HashTable< Key, Value, Hash, Equal >::end() const noexcept
{
  return ConstIterator(this, capacity());
}

template< class Key, class Value, class Hash, class Equal >
typename HashTable< Key, Value, Hash, Equal >::ConstIterator HashTable< Key, Value, Hash, Equal >::cbegin() const noexcept
{
  return ConstIterator(this, 0);
}

template< class Key, class Value, class Hash, class Equal >
typename HashTable< Key, Value, Hash, Equal >::ConstIterator HashTable< Key, Value, Hash, Equal >::cend() const noexcept
{
  return ConstIterator(this, capacity());
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
double HashTable< Key, Value, Hash, Equal >::loadFactor() const noexcept
{
  return static_cast< double >(size_) / static_cast< double >(bucket_count_ * bucket_capacity_);
}

template< class Key, class Value, class Hash, class Equal >
size_t HashTable< Key, Value, Hash, Equal >::spareBucketSize() const noexcept
{
  size_t result = 0;
  const size_t overflow = overflowFirst();

  for (size_t i = 0; i < bucket_capacity_; ++i)
  {
    if (data_[overflow + i].occupied)
    {
      ++result;
    }
  }

  return result;
}

template< class Key, class Value, class Hash, class Equal >
double HashTable< Key, Value, Hash, Equal >::averageBucketSize() const noexcept
{
  return static_cast< double >(size_) / static_cast< double >(bucket_count_);
}

template< class Key, class Value, class Hash, class Equal >
size_t HashTable< Key, Value, Hash, Equal >::maxBucketSize() const noexcept
{
  size_t result = 0;

  for (size_t bucket = 0; bucket < bucket_count_; ++bucket)
  {
    size_t current = 0;
    const size_t first = bucketFirst(bucket);

    for (size_t i = 0; i < bucket_capacity_; ++i)
    {
      if (data_[first + i].occupied)
      {
        ++current;
      }
    }

    if (current > result)
    {
      result = current;
    }
  }

  return result;
}

template< class Key, class Value, class Hash, class Equal >
void HashTable< Key, Value, Hash, Equal >::maxLoadFactor(double value)
{
  if (value <= 0.0)
  {
    throw std::invalid_argument("invalid max load factor");
  }

  max_load_factor_ = value;
}

template< class Key, class Value, class Hash, class Equal >
void HashTable< Key, Value, Hash, Equal >::maxSpareBucketSize(size_t value) noexcept
{
  max_spare_bucket_size_ = value;
}

template< class Key, class Value, class Hash, class Equal >
void HashTable< Key, Value, Hash, Equal >::maxAverageBucketSize(double value)
{
  if (value <= 0.0)
  {
    throw std::invalid_argument("invalid max average bucket size");
  }

  max_average_bucket_size_ = value;
}

template< class Key, class Value, class Hash, class Equal >
void HashTable< Key, Value, Hash, Equal >::bucketUpdateFunction(SizeUpdateFunction function)
{
  if (function == nullptr)
  {
    throw std::invalid_argument("invalid bucket update function");
  }

  bucket_update_function_ = function;
}

template< class Key, class Value, class Hash, class Equal >
void HashTable< Key, Value, Hash, Equal >::bucketSizeUpdateFunction(SizeUpdateFunction function)
{
  if (function == nullptr)
  {
    throw std::invalid_argument("invalid bucket size update function");
  }

  bucket_size_update_function_ = function;
}

template< class Key, class Value, class Hash, class Equal >
size_t HashTable< Key, Value, Hash, Equal >::indexOf(const Key& key) const
{
  return hash_(key) % bucket_count_;
}

template< class Key, class Value, class Hash, class Equal >
size_t HashTable< Key, Value, Hash, Equal >::capacity() const noexcept
{
  return (bucket_count_ + 1) * bucket_capacity_;
}

template< class Key, class Value, class Hash, class Equal >
size_t HashTable< Key, Value, Hash, Equal >::bucketFirst(size_t bucket) const noexcept
{
  return bucket * bucket_capacity_;
}

template< class Key, class Value, class Hash, class Equal >
size_t HashTable< Key, Value, Hash, Equal >::overflowFirst() const noexcept
{
  return bucket_count_ * bucket_capacity_;
}

template< class Key, class Value, class Hash, class Equal >
size_t HashTable< Key, Value, Hash, Equal >::defaultSizeUpdate(size_t value)
{
  if (value > std::numeric_limits< size_t >::max() / 2)
  {
    throw std::overflow_error("hash table size update overflow");
  }

  return value * 2;
}

}

#endif
