#ifndef CUCKOO_HPP
#define CUCKOO_HPP

#include <Vector.hpp>
#include <cstddef>
#include <functional>
#include <hashFunctions.hpp>
#include <utility>
#include "cuckooIter.hpp"

namespace karpovich
{
  template< class T >
  using eq = std::equal_to< T >;
  template< class T >
  using hash = Hasher< T >;
  template< class Key, class Value, class Hash1 = hash< Key >, class Hash2 = std::hash< Key >, class Equal = eq< Key > >
  class CuckooTable
  {
  public:
    using valType = std::pair< Key, Value >;
    using entryType = details::CuckooEntry< Key, Value >;

    explicit CuckooTable(size_t capacity = 16);

    CuckooTable(const CuckooTable &other);
    CuckooTable(CuckooTable &&other) noexcept;
    CuckooTable &operator=(const CuckooTable &other);
    CuckooTable &operator=(CuckooTable &&other) noexcept;

    void add(const Key &k, const Value &v);
    void drop(const Key &k);
    Value &get(const Key &k);
    const Value &get(const Key &k) const;

    bool has(const Key &k) const noexcept;
    void rehash(size_t newCapacity);
    void clear() noexcept;
    size_t size() const noexcept;
    bool empty() const noexcept;
    void swap(CuckooTable &other) noexcept;
    using iter_t = CuckooIter< Key, Value, Hash1, Hash2, Equal >;
    using citer_t = CuckooConstIter< Key, Value, Hash1, Hash2, Equal >;

    iter_t begin() noexcept;
    iter_t end() noexcept;
    citer_t begin() const noexcept;
    citer_t end() const noexcept;
    citer_t cbegin() const noexcept;
    citer_t cend() const noexcept;

    double loadFactor() const noexcept;
    void setMaxLoadFactor(double maxLf) noexcept;
    void setMaxKickCount(size_t maxKicks) noexcept;

  private:
    Vector< entryType > table1_;
    Vector< entryType > table2_;
    size_t capacity_;
    size_t size_;
    Hash1 hasher1_;
    Hash2 hasher2_;
    Equal comparator_;
    double maxLoadFactor_;
    size_t maxKickCount_;
  };
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
karpovich::CuckooTable< Key, Value, Hash1, Hash2, Equal >::CuckooTable(size_t capacity):
  table1_(),
  table2_(),
  capacity_(capacity),
  size_(0),
  hasher1_(),
  hasher2_(),
  comparator_(),
  maxLoadFactor_(0.5),
  maxKickCount_(capacity)
{
  table1_.resize(capacity);
  table2_.resize(capacity);
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
karpovich::CuckooTable< Key, Value, Hash1, Hash2, Equal >::CuckooTable(const CuckooTable &other):
  table1_(other.table1_),
  table2_(other.table2_),
  capacity_(other.capacity_),
  size_(other.size_),
  hasher1_(other.hasher1_),
  hasher2_(other.hasher2_),
  comparator_(other.comparator_),
  maxLoadFactor_(other.maxLoadFactor_),
  maxKickCount_(other.maxKickCount_)
{}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
karpovich::CuckooTable< Key, Value, Hash1, Hash2, Equal >::CuckooTable(CuckooTable &&other) noexcept:
  table1_(std::move(other.table1_)),
  table2_(std::move(other.table2_)),
  capacity_(other.capacity_),
  size_(other.size_),
  hasher1_(std::move(other.hasher1_)),
  hasher2_(std::move(other.hasher2_)),
  comparator_(std::move(other.comparator_)),
  maxLoadFactor_(other.maxLoadFactor_),
  maxKickCount_(other.maxKickCount_)
{
  other.capacity_ = 0;
  other.size_ = 0;
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
karpovich::CuckooTable< Key, Value, Hash1, Hash2, Equal > &
karpovich::CuckooTable< Key, Value, Hash1, Hash2, Equal >::operator=(const CuckooTable &other)
{
  if (this != std::addressof(other)) {
    table1_ = other.table1_;
    table2_ = other.table2_;
    capacity_ = other.capacity_;
    size_ = other.size_;
    hasher1_ = other.hasher1_;
    hasher2_ = other.hasher2_;
    comparator_ = other.comparator_;
    maxLoadFactor_ = other.maxLoadFactor_;
    maxKickCount_ = other.maxKickCount_;
  }
  return *this;
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
karpovich::CuckooTable< Key, Value, Hash1, Hash2, Equal > &
karpovich::CuckooTable< Key, Value, Hash1, Hash2, Equal >::operator=(CuckooTable &&other) noexcept
{
  if (this != std::addressof(other)) {
    table1_ = std::move(other.table1_);
    table2_ = std::move(other.table2_);
    capacity_ = other.capacity_;
    size_ = other.size_;
    hasher1_ = std::move(other.hasher1_);
    hasher2_ = std::move(other.hasher2_);
    comparator_ = std::move(other.comparator_);
    maxLoadFactor_ = other.maxLoadFactor_;
    maxKickCount_ = other.maxKickCount_;

    other.capacity_ = 0;
    other.size_ = 0;
  }
  return *this;
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
bool karpovich::CuckooTable< Key, Value, Hash1, Hash2, Equal >::empty() const noexcept
{
  return size_ == 0;
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
size_t karpovich::CuckooTable< Key, Value, Hash1, Hash2, Equal >::size() const noexcept
{
  return size_;
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
double karpovich::CuckooTable< Key, Value, Hash1, Hash2, Equal >::loadFactor() const noexcept
{
  return static_cast< double >(size_) / (capacity_ * 2);
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
void karpovich::CuckooTable< Key, Value, Hash1, Hash2, Equal >::setMaxLoadFactor(double maxLf) noexcept
{
  maxLoadFactor_ = maxLf;
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
void karpovich::CuckooTable< Key, Value, Hash1, Hash2, Equal >::setMaxKickCount(size_t maxKicks) noexcept
{
  maxKickCount_ = maxKicks;
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
void karpovich::CuckooTable< Key, Value, Hash1, Hash2, Equal >::clear() noexcept
{
  table1_ = Vector< entryType >();
  table2_ = Vector< entryType >();

  table1_.resize(capacity_);
  table2_.resize(capacity_);
  size_ = 0;
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
void karpovich::CuckooTable< Key, Value, Hash1, Hash2, Equal >::swap(CuckooTable &other) noexcept
{
  std::swap(table1_, other.table1_);
  std::swap(table2_, other.table2_);
  std::swap(capacity_, other.capacity_);
  std::swap(size_, other.size_);
  std::swap(hasher1_, other.hasher1_);
  std::swap(hasher2_, other.hasher2_);
  std::swap(comparator_, other.comparator_);
  std::swap(maxLoadFactor_, other.maxLoadFactor_);
  std::swap(maxKickCount_, other.maxKickCount_);
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
bool karpovich::CuckooTable< Key, Value, Hash1, Hash2, Equal >::has(const Key &k) const noexcept
{
  size_t h1 = hasher1_(k) % capacity_;
  size_t h2 = hasher2_(k) % capacity_;
  if (table1_[h1].occupied_ && comparator_(table1_[h1].data_.first, k)) {
    return true;
  }
  if (table2_[h2].occupied_ && comparator_(table2_[h2].data_.first, k)) {
    return true;
  }
  return false;
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
Value &karpovich::CuckooTable< Key, Value, Hash1, Hash2, Equal >::get(const Key &k)
{
  size_t h1 = hasher1_(k) % capacity_;
  size_t h2 = hasher2_(k) % capacity_;
  if (table1_[h1].occupied_ && comparator_(table1_[h1].data_.first, k)) {
    return table1_[h1].data_.second;
  }
  if (table2_[h2].occupied_ && comparator_(table2_[h2].data_.first, k)) {
    return table2_[h2].data_.second;
  }
  throw std::out_of_range("Key not found");
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
const Value &karpovich::CuckooTable< Key, Value, Hash1, Hash2, Equal >::get(const Key &k) const
{
  size_t h1 = hasher1_(k) % capacity_;
  size_t h2 = hasher2_(k) % capacity_;
  if (table1_[h1].occupied_ && comparator_(table1_[h1].data_.first, k)) {
    return table1_[h1].data_.second;
  }
  if (table2_[h2].occupied_ && comparator_(table2_[h2].data_.first, k)) {
    return table2_[h2].data_.second;
  }
  throw std::out_of_range("Key not found");
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
void karpovich::CuckooTable< Key, Value, Hash1, Hash2, Equal >::add(const Key &k, const Value &v)
{
  if (has(k)) {
    get(k) = v;
    return;
  }
  if (loadFactor() >= maxLoadFactor_) {
    rehash(capacity_ * 2);
  }
  valType current(k, v);
  bool firstTable = true;
  for (size_t kick = 0; kick < maxKickCount_; ++kick) {
    if (firstTable) {
      size_t pos = hasher1_(current.first) % capacity_;
      if (!table1_[pos].occupied_) {
        table1_[pos] = entryType(current.first, current.second);
        ++size_;
        return;
      }
      std::swap(current, table1_[pos].data_);
      firstTable = false;
    } else {
      size_t pos = hasher2_(current.first) % capacity_;
      if (!table2_[pos].occupied_) {
        table2_[pos] = entryType(current.first, current.second);
        ++size_;
        return;
      }
      std::swap(current, table2_[pos].data_);
      firstTable = true;
    }
  }
  rehash(capacity_ * 2);
  add(current.first, current.second);
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
void karpovich::CuckooTable< Key, Value, Hash1, Hash2, Equal >::drop(const Key &k)
{
  size_t h1 = hasher1_(k) % capacity_;
  size_t h2 = hasher2_(k) % capacity_;
  bool found = false;
  if (table1_[h1].occupied_ && comparator_(table1_[h1].data_.first, k)) {
    table1_[h1].occupied_ = false;
    --size_;
    found = true;
  } else if (table2_[h2].occupied_ && comparator_(table2_[h2].data_.first, k)) {
    table2_[h2].occupied_ = false;
    --size_;
    found = true;
  }
  if (!found) {
    throw std::out_of_range("Key not found");
  }
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
void karpovich::CuckooTable< Key, Value, Hash1, Hash2, Equal >::rehash(size_t newCapacity)
{
  Vector< entryType > oldTable1 = table1_;
  Vector< entryType > oldTable2 = table2_;
  table1_ = Vector< entryType >();
  table2_ = Vector< entryType >();
  table1_.resize(newCapacity);
  table2_.resize(newCapacity);
  capacity_ = newCapacity;
  size_ = 0;
  maxKickCount_ = newCapacity;

  for (size_t i = 0; i < oldTable1.getSize(); ++i) {
    if (oldTable1[i].occupied_) {
      add(oldTable1[i].data_.first, oldTable1[i].data_.second);
    }
  }
  for (size_t i = 0; i < oldTable2.getSize(); ++i) {
    if (oldTable2[i].occupied_) {
      add(oldTable2[i].data_.first, oldTable2[i].data_.second);
    }
  }
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
typename karpovich::CuckooTable< Key, Value, Hash1, Hash2, Equal >::iter_t
karpovich::CuckooTable< Key, Value, Hash1, Hash2, Equal >::begin() noexcept
{
  return iter_t(std::addressof(table1_), std::addressof(table2_), capacity_, 0, 0);
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
typename karpovich::CuckooTable< Key, Value, Hash1, Hash2, Equal >::iter_t
karpovich::CuckooTable< Key, Value, Hash1, Hash2, Equal >::end() noexcept
{
  return iter_t();
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
typename karpovich::CuckooTable< Key, Value, Hash1, Hash2, Equal >::citer_t
karpovich::CuckooTable< Key, Value, Hash1, Hash2, Equal >::begin() const noexcept
{
  return citer_t(std::addressof(table1_), std::addressof(table2_), capacity_, 0, 0);
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
typename karpovich::CuckooTable< Key, Value, Hash1, Hash2, Equal >::citer_t
karpovich::CuckooTable< Key, Value, Hash1, Hash2, Equal >::end() const noexcept
{
  return citer_t();
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
typename karpovich::CuckooTable< Key, Value, Hash1, Hash2, Equal >::citer_t
karpovich::CuckooTable< Key, Value, Hash1, Hash2, Equal >::cbegin() const noexcept
{
  return citer_t(std::addressof(table1_), std::addressof(table2_), capacity_, 0, 0);
}

template< class Key, class Value, class Hash1, class Hash2, class Equal >
typename karpovich::CuckooTable< Key, Value, Hash1, Hash2, Equal >::citer_t
karpovich::CuckooTable< Key, Value, Hash1, Hash2, Equal >::cend() const noexcept
{
  return citer_t();
}

#endif
