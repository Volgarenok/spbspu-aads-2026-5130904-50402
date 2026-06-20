#ifndef MATVEEV_GAMEWORLD_HASH_TABLE_HPP
#define MATVEEV_GAMEWORLD_HASH_TABLE_HPP

#include <cstddef>
#include <limits>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace matveev
{

template< class Key, class Value >
struct HashTableItem
{
  HashTableItem():
    key(),
    value(),
    occupied(false),
    psl(0)
  {}

  Key key;
  Value value;
  bool occupied;
  std::size_t psl;
};

template< class Key, class Value, class Hash, class Equal >
class HashTable
{
public:
  template< bool IsConst >
  class BasicIterator
  {
    friend class HashTable;

    using TablePointer = typename std::conditional< IsConst, const HashTable*, HashTable* >::type;
    using ItemReference = typename std::conditional< IsConst,
      const HashTableItem< Key, Value >&, HashTableItem< Key, Value >& >::type;
    using ItemPointer = typename std::conditional< IsConst,
      const HashTableItem< Key, Value >*, HashTableItem< Key, Value >* >::type;

  public:
    BasicIterator():
      table_(nullptr),
      pos_(0)
    {}

    ItemReference operator*() const
    {
      return table_->data_[pos_];
    }

    ItemPointer operator->() const
    {
      return std::addressof(table_->data_[pos_]);
    }

    BasicIterator& operator++()
    {
      if (table_ != nullptr && pos_ < table_->capacity_)
      {
        ++pos_;
        skipEmpty();
      }

      return *this;
    }

    BasicIterator operator++(int)
    {
      BasicIterator result(*this);
      ++(*this);
      return result;
    }

    bool operator==(const BasicIterator& other) const
    {
      return table_ == other.table_ && pos_ == other.pos_;
    }

    bool operator!=(const BasicIterator& other) const
    {
      return !(*this == other);
    }

  private:
    BasicIterator(TablePointer table, std::size_t pos):
      table_(table),
      pos_(pos)
    {
      skipEmpty();
    }

    void skipEmpty() noexcept
    {
      while (table_ != nullptr && pos_ < table_->capacity_ && !table_->data_[pos_].occupied)
      {
        ++pos_;
      }
    }

    TablePointer table_;
    std::size_t pos_;
  };

  using Iterator = BasicIterator< false >;
  using ConstIterator = BasicIterator< true >;
  using iterator = Iterator;
  using const_iterator = ConstIterator;

  HashTable():
    HashTable(8)
  {}

  explicit HashTable(std::size_t capacity):
    data_(nullptr),
    capacity_(roundUpPow2(capacity)),
    size_(0),
    max_load_factor_(0.7),
    hash_(),
    equal_()
  {
    data_ = new HashTableItem< Key, Value >[capacity_];
  }

  HashTable(const HashTable& other):
    data_(nullptr),
    capacity_(other.capacity_),
    size_(other.size_),
    max_load_factor_(other.max_load_factor_),
    hash_(other.hash_),
    equal_(other.equal_)
  {
    data_ = new HashTableItem< Key, Value >[capacity_];

    try
    {
      for (std::size_t i = 0; i < capacity_; ++i)
      {
        data_[i] = other.data_[i];
      }
    }
    catch (...)
    {
      delete[] data_;
      throw;
    }
  }

  HashTable(HashTable&& other) noexcept:
    data_(other.data_),
    capacity_(other.capacity_),
    size_(other.size_),
    max_load_factor_(other.max_load_factor_),
    hash_(std::move(other.hash_)),
    equal_(std::move(other.equal_))
  {
    other.data_ = nullptr;
    other.capacity_ = 0;
    other.size_ = 0;
  }

  HashTable& operator=(const HashTable& other)
  {
    if (this != &other)
    {
      HashTable temp(other);
      swap(temp);
    }

    return *this;
  }

  HashTable& operator=(HashTable&& other) noexcept
  {
    if (this != &other)
    {
      delete[] data_;

      data_ = other.data_;
      capacity_ = other.capacity_;
      size_ = other.size_;
      max_load_factor_ = other.max_load_factor_;
      hash_ = std::move(other.hash_);
      equal_ = std::move(other.equal_);

      other.data_ = nullptr;
      other.capacity_ = 0;
      other.size_ = 0;
    }

    return *this;
  }

  ~HashTable()
  {
    delete[] data_;
  }

  void add(const Key& key, const Value& value)
  {
    std::size_t index = 0;

    if (findSlot(key, index))
    {
      throw std::logic_error("key already exists");
    }

    growIfNeeded();
    insertUnsafe(key, value);
  }

  bool has(const Key& key) const
  {
    std::size_t index = 0;
    return findSlot(key, index);
  }

  Value& at(const Key& key)
  {
    std::size_t index = 0;

    if (!findSlot(key, index))
    {
      throw std::out_of_range("key not found");
    }

    return data_[index].value;
  }

  const Value& at(const Key& key) const
  {
    std::size_t index = 0;

    if (!findSlot(key, index))
    {
      throw std::out_of_range("key not found");
    }

    return data_[index].value;
  }

  Value drop(const Key& key)
  {
    using std::swap;

    std::size_t index = 0;

    if (!findSlot(key, index))
    {
      throw std::out_of_range("key not found");
    }

    Value result = std::move(data_[index].value);

    std::size_t current = index;
    std::size_t next = (current + 1) & (capacity_ - 1);

    while (data_[next].occupied && data_[next].psl > 0)
    {
      swap(data_[current].key, data_[next].key);
      swap(data_[current].value, data_[next].value);
      data_[current].psl = data_[next].psl - 1;
      data_[current].occupied = true;

      current = next;
      next = (next + 1) & (capacity_ - 1);
    }

    data_[current].occupied = false;
    data_[current].psl = 0;
    data_[current].key = Key();

    --size_;
    return result;
  }

  void clear() noexcept
  {
    for (std::size_t i = 0; i < capacity_; ++i)
    {
      data_[i].occupied = false;
      data_[i].psl = 0;
    }

    size_ = 0;
  }

  void swap(HashTable& other) noexcept
  {
    using std::swap;

    swap(data_, other.data_);
    swap(capacity_, other.capacity_);
    swap(size_, other.size_);
    swap(max_load_factor_, other.max_load_factor_);
    swap(hash_, other.hash_);
    swap(equal_, other.equal_);
  }

  void rehash(std::size_t capacity)
  {
    std::size_t target = roundUpPow2(capacity);

    while (static_cast< double >(target) < static_cast< double >(size_ + 1) / max_load_factor_)
    {
      target = doubled(target);
    }

    HashTable temp(target);
    temp.max_load_factor_ = max_load_factor_;

    for (std::size_t i = 0; i < capacity_; ++i)
    {
      if (data_[i].occupied)
      {
        temp.insertUnsafe(data_[i].key, data_[i].value);
      }
    }

    swap(temp);
  }

  Iterator begin() noexcept
  {
    return Iterator(this, 0);
  }

  Iterator end() noexcept
  {
    return Iterator(this, capacity_);
  }

  ConstIterator begin() const noexcept
  {
    return ConstIterator(this, 0);
  }

  ConstIterator end() const noexcept
  {
    return ConstIterator(this, capacity_);
  }

  ConstIterator cbegin() const noexcept
  {
    return ConstIterator(this, 0);
  }

  ConstIterator cend() const noexcept
  {
    return ConstIterator(this, capacity_);
  }

  std::size_t size() const noexcept
  {
    return size_;
  }

  std::size_t capacity() const noexcept
  {
    return capacity_;
  }

  bool empty() const noexcept
  {
    return size_ == 0;
  }

  double loadFactor() const noexcept
  {
    if (capacity_ == 0)
    {
      return 0.0;
    }

    return static_cast< double >(size_) / static_cast< double >(capacity_);
  }

  std::size_t maxProbeLength() const noexcept
  {
    std::size_t result = 0;

    for (std::size_t i = 0; i < capacity_; ++i)
    {
      if (data_[i].occupied && data_[i].psl > result)
      {
        result = data_[i].psl;
      }
    }

    return result;
  }

  void maxLoadFactor(double value)
  {
    if (value <= 0.0 || value >= 1.0)
    {
      throw std::invalid_argument("max load factor must be in (0, 1)");
    }

    max_load_factor_ = value;
    growIfNeeded();
  }

  double maxLoadFactor() const noexcept
  {
    return max_load_factor_;
  }

private:
  static std::size_t roundUpPow2(std::size_t value) noexcept
  {
    std::size_t result = 1;

    while (result < value)
    {
      result <<= 1;
    }

    return result;
  }

  static std::size_t doubled(std::size_t value)
  {
    if (value > std::numeric_limits< std::size_t >::max() / 2)
    {
      throw std::overflow_error("hash table capacity overflow");
    }

    return value * 2;
  }

  std::size_t homeOf(const Key& key) const
  {
    return hash_(key) & (capacity_ - 1);
  }

  bool findSlot(const Key& key, std::size_t& index) const
  {
    if (capacity_ == 0)
    {
      return false;
    }

    std::size_t pos = homeOf(key);
    std::size_t psl = 0;

    while (data_[pos].occupied)
    {
      if (data_[pos].psl < psl)
      {
        return false;
      }

      if (equal_(data_[pos].key, key))
      {
        index = pos;
        return true;
      }

      pos = (pos + 1) & (capacity_ - 1);
      ++psl;
    }

    return false;
  }

  void insertUnsafe(const Key& key, const Value& value)
  {
    using std::swap;

    Key cur_key = key;
    Value cur_value = value;
    std::size_t cur_psl = 0;
    std::size_t pos = homeOf(key);

    while (data_[pos].occupied)
    {
      if (data_[pos].psl < cur_psl)
      {
        swap(data_[pos].key, cur_key);
        swap(data_[pos].value, cur_value);
        swap(data_[pos].psl, cur_psl);
      }

      pos = (pos + 1) & (capacity_ - 1);
      ++cur_psl;
    }

    data_[pos].key = std::move(cur_key);
    data_[pos].value = std::move(cur_value);
    data_[pos].psl = cur_psl;
    data_[pos].occupied = true;
    ++size_;
  }

  void growIfNeeded()
  {
    if (capacity_ == 0)
    {
      rehash(8);
    }

    const double required = static_cast< double >(size_ + 1) / max_load_factor_;

    if (static_cast< double >(capacity_) >= required)
    {
      return;
    }

    std::size_t new_capacity = capacity_;

    while (static_cast< double >(new_capacity) < required)
    {
      new_capacity = doubled(new_capacity);
    }

    rehash(new_capacity);
  }

  HashTableItem< Key, Value >* data_;
  std::size_t capacity_;
  std::size_t size_;
  double max_load_factor_;
  Hash hash_;
  Equal equal_;
};
}

#endif
