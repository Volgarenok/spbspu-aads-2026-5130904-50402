#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP
#include "vector.hpp"
#include <hasher.hpp>

namespace muhamadiarov
{
  template < class Key, class Value >
  struct Slot
  {
    std::pair< Key, Value > value_;
    size_t psl_;
    bool occupied_;
    Slot();
    void swap(Slot& other) noexcept;
    void clear() noexcept;
  };

  template < class Key, class Value, class Hash, class Equal >
  class RTIter;

  template < class Key, class Value, class Hash, class Equal >
  class RTCIter;

  template < class Key, class Value, class Hash = SipHash< Key >, class Equal = std::equal_to< Key > >
  class RobinTable
  {
  public:
    using Iterator = RTIter < Key, Value, Hash, Equal >;
    using ConstIterator = RTCIter< Key, Value, Hash, Equal >;

    RobinTable();
    explicit RobinTable(size_t slots, float maxLoad = 0.7f);
    RobinTable(const RobinTable& other);
    RobinTable(RobinTable&& other) noexcept;
    RobinTable& operator=(const RobinTable& other);
    RobinTable& operator=(RobinTable&& other) noexcept;

    ~RobinTable();

    void add(Key k, Value v);
    void drop(Key k);
    Value& get(Key k);
    const Value& get(Key k) const;
    bool has(Key k) const noexcept;
    void rehash(size_t slots);

    void clear() noexcept;

    size_t size() const noexcept;
    bool empty() const noexcept;
    float maxLoadFactor() const noexcept;

    Iterator begin() noexcept;
    Iterator end() noexcept;
    ConstIterator begin() const noexcept;
    ConstIterator end() const noexcept;
    ConstIterator cbegin() const noexcept;
    ConstIterator cend() const noexcept;

    void swap(RobinTable& other) noexcept;
  private:
    Vector< Slot< Key, Value > > data_;
    Hash hasher_;
    Equal equal_;
    size_t capacity_;
    size_t size_;
    float maxLoad_;

    size_t findSlot(const Key& k) const;
    friend class RTIter<Key, Value, Hash, Equal>;
    friend class RTCIter<Key, Value, Hash, Equal>;
  };

  template < class Key, class Value, class Hash, class Equal >
  class RTIter
  {
  public:
    RTIter();
    RTIter(Vector< Slot< Key, Value > >* data, size_t id);

    RTIter& operator++();
    RTIter& operator++(int);

    RTIter& operator--();
    RTIter& operator--(int);

    bool operator==(const RTIter& other) const noexcept;
    bool operator!=(const RTIter& other) const noexcept;

    std::pair< Key, Value >& operator*() noexcept;
    std::pair< Key, Value >* operator->() noexcept;
  private:
    Vector< Slot< Key, Value > >* data_;
    size_t index_;

    void next();
    void prev();
  };

  template < class Key, class Value, class Hash, class Equal >
  class RTCIter
  {
  public:
    RTCIter();
    RTCIter(const Vector< Slot< Key, Value > >* data, size_t id);

    RTCIter& operator++();
    RTCIter& operator++(int);

    RTCIter& operator--();
    RTCIter& operator--(int);

    bool operator==(const RTCIter& other) const noexcept;
    bool operator!=(const RTCIter& other) const noexcept;

    const std::pair< Key, Value >& operator*() const noexcept;
    const std::pair< Key, Value >* operator->() const noexcept;
  private:
    const Vector< Slot< Key, Value > >* data_;
    size_t index_;

    void next();
    void prev();
  };
}
namespace muh = muhamadiarov;

template < class K, class V >
muh::Slot< K, V >::Slot():
  value_(),
  psl_(0),
  occupied_(false)
{}


template < class K, class V >
void muh::Slot< K, V >::swap(Slot& other) noexcept
{
  std::swap(value_, other.value_);
  std::swap(psl_, other.psl_);
  std::swap(occupied_, other.occupied_);
}

template < class K, class V >
void muh::Slot< K, V >::clear() noexcept
{
  occupied_ = false;
  psl_ = 0;
}

template < class K, class V, class H, class E >
muh::RobinTable< K, V, H, E >::RobinTable():
    capacity_(8),
    size_(0),
    maxLoad_(0.7f)
{
  data_.reserve(capacity_);
  for (size_t i = 0; i < capacity_; ++i)
  {
    data_.pushBack(Slot< K, V >());
  }
}

template < class K, class V, class H, class E >
muh::RobinTable< K, V, H, E >::RobinTable(size_t slots, float maxLoad):
  capacity_(slots),
  size_(0),
  maxLoad_(maxLoad)
{
  data_.reserve(capacity_);
  for (size_t i = 0; i < capacity_; ++i)
  {
    data_.pushBack(Slot< K, V >());
  }
}

template < class K, class V, class H, class E >
muh::RobinTable< K, V, H, E >::RobinTable(const RobinTable& other):
  hasher_(other.hasher_),
  equal_(other.equal_),
  capacity_(other.capacity_),
  size_(other.size_),
  maxLoad_(other.maxLoad_)
{
  data_.reserve(capacity_);
  for (size_t i = 0; i < capacity_; ++i)
  {
    Slot< K, V > node;
    if (other.data_[i].occupied_)
    {
      node.value_ = other.data_[i].value_;
      node.psl_ = other.data_[i].psl_;
      node.occupied_ = true;
    }
    data_.pushBack(node);
  }
}

template < class K, class V, class H, class E >
muh::RobinTable< K, V, H, E >::RobinTable(RobinTable&& other) noexcept:
  data_(std::move(other.data_)),
  hasher_(std::move(other.hasher_)),
  equal_(std::move(other.equal_)),
  capacity_(other.capacity_),
  size_(other.size_),
  maxLoad_(other.maxLoad_)
{
  other.capacity_ = 0;
  other.size_ = 0;
}

template < class K, class V, class H, class E >
muh::RobinTable< K, V, H, E >& muh::RobinTable< K, V, H, E >::operator=(
  const RobinTable& other
)
{
  if (this != &other)
  {
    RobinTable copy(other);
    swap(copy);
  }
  return *this;
}

template < class K, class V, class H, class E >
muh::RobinTable< K, V, H, E >& muh::RobinTable< K, V, H, E >::operator=(
  RobinTable&& other
) noexcept
{
  if (this != &other)
  {
    RobinTable temp(std::move(other));
    swap(temp);
  }
  return *this;
}

template < class K, class V, class H, class E >
muh::RobinTable< K, V, H, E >::~RobinTable()
{}

template < class K, class V, class H, class E >
void muh::RobinTable< K, V, H, E >::add(K k, V v)
{
  if (capacity_ == 0 || (size_ + 1) > capacity_ * maxLoad_)
  {
    rehash(capacity_ == 0 ? 8 : capacity_ * 2);
  }
  Slot< K, V > incoming;
  incoming.value_ = {std::move(k), std::move(v)};
  incoming.psl_ = 0;
  incoming.occupied_ = true;

  size_t slot = hasher_(incoming.value_.first) % capacity_;
  for (size_t i = 0; i < capacity_; ++i)
  {
    Slot< K, V >& curr = data_[slot];
    if (!curr.occupied_)
    {
      curr = std::move(incoming);
      ++size_;
      return;
    }
    if (equal_(curr.value_.first, k))
    {
      curr.value_.second = std::move(v);
      return;
    }
    if (curr.psl_ < incoming.psl_)
    {
      incoming.swap(curr);
    }
    ++incoming.psl_;
    slot = (slot + 1) % capacity_;
  }
}

template < class K, class V, class H, class E >
void muh::RobinTable< K, V, H, E >::drop(K k)
{
  size_t slot = findSlot(k);
  if (slot == capacity_)
  {
    return;
  }
  data_[slot].clear();
  --size_;
  size_t next = (slot + 1) % capacity_;
  while (data_[next].occupied_ && data_[next].psl_ > 0)
  {
    data_[slot] = std::move(data_[next]);
    data_[slot].psl_ -= 1;
    data_[next].clear();
    slot = next;
    next = (next + 1) % capacity_;
  }
}

template < class K, class V, class H, class E >
V& muh::RobinTable< K, V, H, E >::get(K k)
{
  size_t slot = findSlot(k);
  if (slot == capacity_)
  {
    throw std::out_of_range("RobinTable::get: key out of range");
  }
  return data_[slot].value_.second;
}

template < class K, class V, class H, class E >
const V& muh::RobinTable< K, V, H, E >::get(K k) const
{
  size_t slot = findSlot(k);
  if (slot == capacity_)
  {
    throw std::out_of_range("RobinTable::get: key out of range");
  }
  return data_[slot].value_.second;
}

template < class K, class V, class H, class E >
bool muh::RobinTable< K, V, H, E >::has(K k) const noexcept
{
  return findSlot(k) != capacity_;
}

template < class K, class V, class H, class E >
void muh::RobinTable< K, V, H, E >::rehash(size_t slots)
{
  if (slots <= capacity_)
  {
    return;
  }
  RobinTable fresh(slots, maxLoad_);
  for (size_t i = 0; i < capacity_; ++i)
  {
    if (data_[i].occupied_)
    {
      fresh.add(data_[i].value_.first, data_[i].value_.second);
    }
  }
  swap(fresh);
}

template < class K, class V, class H, class E >
size_t muh::RobinTable< K, V, H, E >::findSlot(const K& k) const
{
  if (capacity_ == 0)
  {
    return capacity_;
  }
  size_t slot = hasher_(k) % capacity_;
  for (size_t psl = 0; psl < capacity_; ++psl)
  {
    const Slot< K, V >& cur = data_[slot];
    if (!cur.occupied_ || cur.psl_ < psl)
    {
      return capacity_;
    }
    if (equal_(cur.value_.first, k))
    {
      return slot;
    }
    slot = (slot + 1) % capacity_;
  }
  return capacity_;
}

template < class K, class V, class H, class E >
void muh::RobinTable< K, V, H, E >::clear() noexcept
{
  if (size_ == 0)
  {
    return;
  }
  for (size_t i = 0; i < capacity_; ++i)
  {
    data_[i].clear();
  }
  size_ = 0;
}

template < class K, class V, class H, class E >
size_t muh::RobinTable< K, V, H, E >::size() const noexcept
{
  return size_;
}

template < class K, class V, class H, class E >
bool muh::RobinTable< K, V, H, E >::empty() const noexcept
{
  return size_ == 0;
}

template < class K, class V, class H, class E >
float muh::RobinTable< K, V, H, E >::maxLoadFactor() const noexcept
{
  return maxLoad_;
}

template < class K, class V, class H, class E >
void muh::RobinTable< K, V, H, E >::swap(RobinTable& other) noexcept
{
  std::swap(data_, other.data_);
  std::swap(hasher_, other.hasher_);
  std::swap(equal_, other.equal_);
  std::swap(capacity_, other.capacity_);
  std::swap(size_, other.size_);
  std::swap(maxLoad_, other.maxLoad_);
}

template < class K, class V, class H, class E >
muh::RTIter< K, V, H, E > muh::RobinTable< K, V, H, E >::begin() noexcept
{
  return RTIter< K, V, H, E >(&data_, 0);
}

template < class K, class V, class H, class E >
muh::RTIter< K, V, H, E > muh::RobinTable< K, V, H, E >::end() noexcept
{
  return RTIter< K, V, H, E >(&data_, capacity_);
}

template < class K, class V, class H, class E >
muh::RTCIter< K, V, H, E > muh::RobinTable< K, V, H, E >::begin() const noexcept
{
  return RTCIter< K, V, H, E >(&data_, 0);
}
template < class K, class V, class H, class E >
muh::RTCIter< K, V, H, E > muh::RobinTable< K, V, H, E >::end() const noexcept
{
  return RTCIter< K, V, H, E >(&data_, capacity_);
}
template< class K, class V, class H, class E >
muh::RTCIter< K, V, H, E > muh::RobinTable< K, V, H, E >::cbegin() const noexcept
{
  return RTCIter< K, V, H, E >(&data_, 0);
}

template< class K, class V, class H, class E >
muh::RTCIter< K, V, H, E > muh::RobinTable< K, V, H, E >::cend() const noexcept
{
  return RTCIter< K, V, H, E >(&data_, capacity_);
};

template< class K, class V, class H, class E >
muh::RTIter< K, V, H, E >::RTIter():
  data_(nullptr),
  index_(0)
{}

template< class K, class V, class H, class E >
muh::RTIter< K, V, H, E >::RTIter(Vector< Slot< K, V > >* data, size_t id):
  data_(data),
  index_(id)
{
  next();
}

template< class K, class V, class H, class E >
void muh::RTIter< K, V, H, E >::next()
{
  while (data_ && index_ < data_->capacity() && !(*data_)[index_].occupied_)
  {
    ++index_;
  }
}

template < class K, class V, class H, class E >
void muh::RTIter< K, V, H, E >::prev()
{
  while (data_ && index_ > 0 && !(*data_)[index_].occupied_)
  {
    --index_;
  }
}

template < class K, class V, class H, class E >
muh::RTIter< K, V, H, E >& muh::RTIter< K, V, H, E >::operator++()
{
  ++index_;
  next();
  return *this;
}

template < class K, class V, class H, class E >
muh::RTIter< K, V, H, E >& muh::RTIter< K, V, H, E >::operator++(int)
{
  RTIter< K, V, H, E > temp = *this;
  ++(*this);
  return temp;
}

template < class K, class V, class H, class E >
muh::RTIter< K, V, H, E >& muh::RTIter< K, V, H, E >::operator--()
{
  --index_;
  prev();
  return *this;
}

template < class K, class V, class H, class E >
muh::RTIter< K, V, H, E >& muh::RTIter< K, V, H, E >::operator--(int)
{
  RTIter< K, V, H, E > temp = *this;
  --(*this);
  return *this;
}

template < class K, class V, class H, class E >
bool muh::RTIter< K, V, H, E >::operator==(const RTIter& other) const noexcept
{
  return data_ == other.data_ && index_ == other.index_;
}

template < class K, class V, class H, class E >
bool muh::RTIter< K, V, H, E >::operator!=(const RTIter& other) const noexcept
{
  return !(*this == other);
}

template < class K, class V, class H, class E >
std::pair< K, V >& muh::RTIter< K, V, H, E >::operator*() noexcept
{
  return (*data_)[index_].value_;
}

template < class K, class V, class H, class E >
std::pair< K, V >* muh::RTIter< K, V, H, E >::operator->() noexcept
{
  return &(*data_)[index_].value_;
}

template< class K, class V, class H, class E >
muh::RTCIter< K, V, H, E >::RTCIter():
  data_(nullptr),
  index_(0)
{}

template< class K, class V, class H, class E >
muh::RTCIter< K, V, H, E >::RTCIter(const Vector< Slot< K, V > >* data, size_t id):
  data_(data),
  index_(id)
{
  next();
}

template< class K, class V, class H, class E >
void muh::RTCIter< K, V, H, E >::next()
{
  while (data_ && index_ < data_->capacity() && !(*data_)[index_].occupied_)
  {
    ++index_;
  }
}

template < class K, class V, class H, class E >
void muh::RTCIter< K, V, H, E >::prev()
{
  while (data_ && index_ > 0 && !(*data_)[index_].occupied_)
  {
    --index_;
  }
}

template < class K, class V, class H, class E >
muh::RTCIter< K, V, H, E >& muh::RTCIter< K, V, H, E >::operator++()
{
  ++index_;
  next();
  return *this;
}

template < class K, class V, class H, class E >
muh::RTCIter< K, V, H, E >& muh::RTCIter< K, V, H, E >::operator++(int)
{
  RTCIter< K, V, H, E > temp = *this;
  ++(*this);
  return temp;
}

template < class K, class V, class H, class E >
muh::RTCIter< K, V, H, E >& muh::RTCIter< K, V, H, E >::operator--()
{
  --index_;
  prev();
  return *this;
}

template < class K, class V, class H, class E >
muh::RTCIter< K, V, H, E >& muh::RTCIter< K, V, H, E >::operator--(int)
{
  RTCIter< K, V, H, E > temp = *this;
  --(*this);
  return *this;
}

template < class K, class V, class H, class E >
bool muh::RTCIter< K, V, H, E >::operator==(const RTCIter& other) const noexcept
{
  return data_ == other.data_ && index_ == other.index_;
}

template < class K, class V, class H, class E >
bool muh::RTCIter< K, V, H, E >::operator!=(const RTCIter& other) const noexcept
{
  return !(*this == other);
}

template < class K, class V, class H, class E >
const std::pair< K, V >& muh::RTCIter< K, V, H, E >::operator*() const noexcept
{
  return (*data_)[index_].value_;
}

template < class K, class V, class H, class E >
const std::pair< K, V >* muh::RTCIter< K, V, H, E >::operator->() const noexcept
{
  return &(*data_)[index_].value_;
}
#endif
