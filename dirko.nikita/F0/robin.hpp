#ifndef ROBIN_HPP
#define ROBIN_HPP

#include <Vector.hpp>
#include <algorithm>
#include <cstddef>
#include <functional>
#include <hasher.hpp>
#include <memory>
#include <stdexcept>
#include <utility>
namespace dirko
{
  template< class Key, class Value >
  struct RobinNode
  {
    std::pair< Key, Value > val_;
    size_t psl_;
    bool occupied_;
    RobinNode();
    void swap(RobinNode &other);
  };
  template< class Key, class Value, class Hash, class Equal >
  class RTIter;
  template< class Key, class Value, class Hash, class Equal >
  class RTCIter;
  template< class Key, class Value, class Hash = dirko::SipHasher< Key >, class Equal = std::equal_to< Key > >
  class RobinTable
  {
  public:
    RobinTable();
    explicit RobinTable(size_t slots, float load);
    RobinTable(std::initializer_list< std::pair< Key, Value > > il);

    void add(Key k, Value v);
    void drop(Key k);
    Value &get(Key k);
    const Value &get(Key k) const;
    bool has(Key k) const noexcept;
    void rehash(size_t slots);

    void changeKey(Key from, Key to);

    using RTIt = RTIter< Key, Value, Hash, Equal >;
    using RTCIt = RTCIter< Key, Value, Hash, Equal >;
    RTIt begin() noexcept;
    RTIt end() noexcept;
    RTCIt begin() const noexcept;
    RTCIt end() const noexcept;
    RTCIt cbegin() const noexcept;
    RTCIt cend() const noexcept;

    RTIt getIter(const Key &);
    RTCIt getCIter(const Key &) const;

    void clear() noexcept;
    size_t size() const noexcept;
    bool empty() const noexcept;
    void swap(RobinTable &other) noexcept;

  private:
    Vector< RobinNode< Key, Value > > data_;
    Hash hasher_;
    Equal comparator_;
    size_t slots_;
    size_t elements_;
    float max_load_;
    friend class RTIter< Key, Value, Hash, Equal >;
    friend class RTCIter< Key, Value, Hash, Equal >;
  };
  template< class Key, class Value, class Hash = dirko::SipHasher< Key >, class Equal = std::equal_to< Key > >
  class RTIter
  {
  public:
    RTIter();
    RTIter(Vector< RobinNode< Key, Value > > *, size_t);

    RTIter &operator++();
    RTIter operator++(int);
    RTIter &operator--();
    RTIter operator--(int);
    bool operator==(const RTIter &other) const noexcept;
    bool operator!=(const RTIter &other) const noexcept;
    std::pair< Key, Value > &operator*() noexcept;

  private:
    Vector< RobinNode< Key, Value > > *data_;
    size_t id_;
    void next();
    void prev();
  };
  template< class Key, class Value, class Hash, class Equal >
  class RTCIter
  {
  public:
    RTCIter();
    RTCIter(const Vector< RobinNode< Key, Value > > *, size_t);

    RTCIter &operator++();
    RTCIter operator++(int);
    RTCIter &operator--();
    RTCIter operator--(int);
    bool operator==(const RTCIter &other) const noexcept;
    bool operator!=(const RTCIter &other) const noexcept;
    const std::pair< Key, Value > &operator*() const noexcept;

  private:
    const Vector< RobinNode< Key, Value > > *data_;
    size_t id_;
    void next();
    void prev();
  };
}

template< class Key, class Value >
dirko::RobinNode< Key, Value >::RobinNode():
  val_(),
  psl_(0),
  occupied_(false)
{}

template< class Key, class Value, class Hash, class Equal >
dirko::RobinTable< Key, Value, Hash, Equal >::RobinTable():
  RobinTable(16, .7)
{}

template< class Key, class Value, class Hash, class Equal >
dirko::RobinTable< Key, Value, Hash, Equal >::RobinTable(size_t slots, float load):
  data_(),
  hasher_(Hash{}),
  comparator_(Equal{}),
  slots_(slots),
  elements_(0),
  max_load_(load)
{
  data_.reserve(slots);
  for (size_t i = 0; i < slots; ++i) {
    data_.pushBack(RobinNode< Key, Value >());
  }
}
template< class Key, class Value, class Hash, class Equal >
size_t dirko::RobinTable< Key, Value, Hash, Equal >::size() const noexcept
{
  return elements_;
}
template< class Key, class Value, class Hash, class Equal >
bool dirko::RobinTable< Key, Value, Hash, Equal >::empty() const noexcept
{
  return !elements_;
}
template< class Key, class Value, class Hash, class Equal >
void dirko::RobinTable< Key, Value, Hash, Equal >::swap(RobinTable &other) noexcept
{
  data_.swap(other.data_);
  std::swap(hasher_, other.hasher_);
  std::swap(comparator_, other.comparator_);
  std::swap(slots_, other.slots_);
  std::swap(elements_, other.elements_);
  std::swap(max_load_, other.max_load_);
}
template< class Key, class Value, class Hash, class Equal >
void dirko::RobinTable< Key, Value, Hash, Equal >::clear() noexcept
{
  for (size_t i = 0; i < slots_; ++i) {
    data_[i].occupied_ = false;
  }
  elements_ = 0;
}

template< class Key, class Value, class Hash, class Equal >
dirko::RobinTable< Key, Value, Hash, Equal >::RobinTable(std::initializer_list< std::pair< Key, Value > > il):
  RobinTable(il.size(), 5)
{
  data_.reserve(il.size());
  for (const std::pair< Key, Value > &v : il) {
    add(v.first, v.second);
  }
}

template< class Key, class Value, class Hash, class Equal >
dirko::RTIter< Key, Value, Hash, Equal > dirko::RobinTable< Key, Value, Hash, Equal >::begin() noexcept
{
  return RTIter< Key, Value, Hash, Equal >(std::addressof(data_), 0);
}
template< class Key, class Value, class Hash, class Equal >
dirko::RTIter< Key, Value, Hash, Equal > dirko::RobinTable< Key, Value, Hash, Equal >::end() noexcept
{
  return RTIter< Key, Value, Hash, Equal >(std::addressof(data_), slots_);
}
template< class Key, class Value, class Hash, class Equal >
dirko::RTCIter< Key, Value, Hash, Equal > dirko::RobinTable< Key, Value, Hash, Equal >::begin() const noexcept
{
  return RTCIter< Key, Value, Hash, Equal >(std::addressof(data_), 0);
}
template< class Key, class Value, class Hash, class Equal >
dirko::RTCIter< Key, Value, Hash, Equal > dirko::RobinTable< Key, Value, Hash, Equal >::end() const noexcept
{
  return RTCIter< Key, Value, Hash, Equal >(std::addressof(data_), slots_);
}
template< class Key, class Value, class Hash, class Equal >
dirko::RTCIter< Key, Value, Hash, Equal > dirko::RobinTable< Key, Value, Hash, Equal >::cbegin() const noexcept
{
  return RTCIter< Key, Value, Hash, Equal >(std::addressof(data_), 0);
}
template< class Key, class Value, class Hash, class Equal >
dirko::RTCIter< Key, Value, Hash, Equal > dirko::RobinTable< Key, Value, Hash, Equal >::cend() const noexcept
{
  return RTCIter< Key, Value, Hash, Equal >(std::addressof(data_), slots_);
}

template< class Key, class Value, class Hash, class Equal >
dirko::RTIter< Key, Value, Hash, Equal > dirko::RobinTable< Key, Value, Hash, Equal >::getIter(const Key &key)
{
  VIter< RobinNode< Key, Value > > val = data_.begin();
  for (; val != data_.end(); ++val) {
    if ((*val).occupied_ && (*val).val_.first == key) {
      break;
    }
  }
  if (val == data_.end()) {
    throw std::invalid_argument("no such key");
  }
  return RTIter< Key, Value, Hash, Equal >(std::addressof(data_), val.getID());
}

template< class Key, class Value, class Hash, class Equal >
dirko::RTCIter< Key, Value, Hash, Equal > dirko::RobinTable< Key, Value, Hash, Equal >::getCIter(const Key &key) const
{
  VCIter< RobinNode< Key, Value > > val = data_.begin();
  for (; val != data_.end(); ++val) {
    if ((*val).occupied_ && (*val).val_.first == key) {
      break;
    }
  }
  if (val == data_.end()) {
    throw std::invalid_argument("no such key");
  }
  return RTCIter< Key, Value, Hash, Equal >(std::addressof(data_), val.getID());
}
template< class Key, class Value, class Hash, class Equal >
void dirko::RobinTable< Key, Value, Hash, Equal >::rehash(size_t slots)
{
  RobinTable< Key, Value, Hash, Equal > cpy(slots, max_load_);
  for (const std::pair< Key, Value > &v : *this) {
    cpy.add(v.first, v.second);
  }
  swap(cpy);
}

template< class Key, class Value, class Hash, class Equal >
void dirko::RobinTable< Key, Value, Hash, Equal >::add(Key k, Value v)
{
  if (has(k)) {
    get(k) = v;
    return;
  }
  if (elements_ >= slots_ * max_load_) {
    rehash(!slots_ ? 16 : slots_ * 2);
  }
  size_t id = hasher_(k) % slots_;
  size_t psl = 0;
  while (data_[id].occupied_) {
    if (data_[id].psl_ < psl) {
      std::swap(k, data_[id].val_.first);
      std::swap(v, data_[id].val_.second);
      std::swap(psl, data_[id].psl_);
    }
    id = (id + 1) % slots_;
    ++psl;
  }
  data_[id].val_ = {k, v};
  data_[id].psl_ = psl;
  data_[id].occupied_ = true;
  ++elements_;
}
template< class Key, class Value, class Hash, class Equal >
void dirko::RobinTable< Key, Value, Hash, Equal >::changeKey(Key from, Key to)
{
  if (has(to)) {
    throw std::invalid_argument("key already exits");
    return;
  }
  if (!has(from)) {
    throw std::invalid_argument("no such element");
    return;
  }
  add(to, std::move(get(from)));
  drop(from);
}
template< class Key, class Value, class Hash, class Equal >
void dirko::RobinTable< Key, Value, Hash, Equal >::drop(Key k)
{
  if (data_.isEmpty()) {
    return;
  }
  if (!has(k)) {
    throw std::invalid_argument("No such key");
  }
  size_t id = hasher_(k) % slots_;
  size_t cycle = id;
  while (data_[id].occupied_) {
    if (comparator_(k, data_[id].val_.first)) {
      size_t curr = id;
      size_t next = (curr + 1) % slots_;
      while (data_[next].occupied_ && data_[next].psl_ > 0) {
        data_[curr].val_ = std::move(data_[next].val_);
        data_[curr].psl_ = data_[next].psl_ - 1;
        curr = next;
        next = (next + 1) % slots_;
      }
      data_[curr].occupied_ = false;
      data_[curr].psl_ = 0;
      --elements_;
      return;
    }
    id = (id + 1) % slots_;
    if (id == cycle) {
      throw std::invalid_argument("No such key");
    }
  }
}

template< class Key, class Value, class Hash, class Equal >
bool dirko::RobinTable< Key, Value, Hash, Equal >::has(Key k) const noexcept
{
  if (data_.isEmpty()) {
    return false;
  }
  size_t id = hasher_(k) % slots_;
  size_t cycle = id;
  while (data_[id].occupied_) {
    if (comparator_(k, data_[id].val_.first)) {
      return true;
    }
    id = (id + 1) % slots_;
    if (id == cycle) {
      return false;
    }
  }
  return false;
}

template< class Key, class Value, class Hash, class Equal >
Value &dirko::RobinTable< Key, Value, Hash, Equal >::get(Key k)
{
  if (!has(k)) {
    throw std::invalid_argument("No such key");
  }
  size_t id = hasher_(k) % slots_;
  size_t cycle = id;
  while (data_[id].occupied_) {
    if (comparator_(k, data_[id].val_.first)) {
      return data_[id].val_.second;
    }
    id = (id + 1) % slots_;
    if (id == cycle) {
      throw std::invalid_argument("No such key");
    }
  }
  throw std::invalid_argument("No such key");
}
template< class Key, class Value, class Hash, class Equal >
const Value &dirko::RobinTable< Key, Value, Hash, Equal >::get(Key k) const
{
  if (!has(k)) {
    throw std::invalid_argument("No such key");
  }
  size_t id = hasher_(k) % slots_;
  size_t cycle = id;
  while (data_[id].occupied_) {
    if (comparator_(k, data_[id].val_.first)) {
      return data_[id].val_.second;
    }
    id = (id + 1) % slots_;
    if (id == cycle) {
      throw std::invalid_argument("No such key");
    }
  }
  throw std::invalid_argument("No such key");
}

template< class Key, class Value, class Hash, class Equal >
dirko::RTIter< Key, Value, Hash, Equal >::RTIter():
  data_(nullptr),
  id_(0)
{}
template< class Key, class Value, class Hash, class Equal >
dirko::RTIter< Key, Value, Hash, Equal >::RTIter(Vector< RobinNode< Key, Value > > *data, size_t id):
  data_(data),
  id_(id)
{
  next();
}

template< class Key, class Value, class Hash, class Equal >
bool dirko::RTIter< Key, Value, Hash, Equal >::operator==(const RTIter< Key, Value, Hash, Equal > &other) const noexcept
{
  if (data_ == nullptr && other.data_ == nullptr) {
    return true;
  }
  if (data_ == nullptr || other.data_ == nullptr) {
    return false;
  }
  if (id_ >= data_->getSize() && other.id_ >= other.data_->getSize()) {
    return true;
  }
  return data_ == other.data_ && other.id_ == id_;
}

template< class Key, class Value, class Hash, class Equal >
bool dirko::RTIter< Key, Value, Hash, Equal >::operator!=(const RTIter< Key, Value, Hash, Equal > &other) const noexcept
{
  return !(*this == other);
}
template< class Key, class Value, class Hash, class Equal >
std::pair< Key, Value > &dirko::RTIter< Key, Value, Hash, Equal >::operator*() noexcept
{
  return (*data_)[id_].val_;
}
template< class Key, class Value, class Hash, class Equal >
dirko::RTIter< Key, Value, Hash, Equal > &dirko::RTIter< Key, Value, Hash, Equal >::operator++()
{
  ++id_;
  next();
  return *this;
}
template< class Key, class Value, class Hash, class Equal >
dirko::RTIter< Key, Value, Hash, Equal > dirko::RTIter< Key, Value, Hash, Equal >::operator++(int)
{
  RTIter< Key, Value, Hash, Equal > ret = *this;
  ++id_;
  next();
  return ret;
}
template< class Key, class Value, class Hash, class Equal >
dirko::RTIter< Key, Value, Hash, Equal > &dirko::RTIter< Key, Value, Hash, Equal >::operator--()
{
  --id_;
  prev();
  return *this;
}
template< class Key, class Value, class Hash, class Equal >
dirko::RTIter< Key, Value, Hash, Equal > dirko::RTIter< Key, Value, Hash, Equal >::operator--(int)
{
  RTIter< Key, Value, Hash, Equal > ret = *this;
  --id_;
  prev();
  return ret;
}
template< class Key, class Value, class Hash, class Equal >
void dirko::RTIter< Key, Value, Hash, Equal >::next()
{
  while (id_ < data_->getSize() && !(*data_)[id_].occupied_) {
    ++id_;
  }
}

template< class Key, class Value, class Hash, class Equal >
void dirko::RTIter< Key, Value, Hash, Equal >::prev()
{
  while (id_ > 1 && !(*data_)[id_].occupied_) {
    --id_;
  }
}
template< class Key, class Value, class Hash, class Equal >
dirko::RTCIter< Key, Value, Hash, Equal >::RTCIter():
  data_(nullptr),
  id_(0)
{}
template< class Key, class Value, class Hash, class Equal >
dirko::RTCIter< Key, Value, Hash, Equal >::RTCIter(const Vector< RobinNode< Key, Value > > *data, size_t id):
  data_(data),
  id_(id)
{
  next();
}

template< class Key, class Value, class Hash, class Equal >
bool dirko::RTCIter< Key, Value, Hash, Equal >::operator==(
    const RTCIter< Key, Value, Hash, Equal > &other) const noexcept
{
  if (data_ == nullptr && other.data_ == nullptr) {
    return true;
  }
  if (data_ == nullptr || other.data_ == nullptr) {
    return false;
  }
  if (id_ >= data_->getSize() && other.id_ >= other.data_->getSize()) {
    return true;
  }
  return data_ == other.data_ && other.id_ == id_;
}

template< class Key, class Value, class Hash, class Equal >
bool dirko::RTCIter< Key, Value, Hash, Equal >::operator!=(
    const RTCIter< Key, Value, Hash, Equal > &other) const noexcept
{
  return !(*this == other);
}
template< class Key, class Value, class Hash, class Equal >
const std::pair< Key, Value > &dirko::RTCIter< Key, Value, Hash, Equal >::operator*() const noexcept
{
  return (*data_)[id_].val_;
}
template< class Key, class Value, class Hash, class Equal >
dirko::RTCIter< Key, Value, Hash, Equal > &dirko::RTCIter< Key, Value, Hash, Equal >::operator++()
{
  ++id_;
  next();
  return *this;
}
template< class Key, class Value, class Hash, class Equal >
dirko::RTCIter< Key, Value, Hash, Equal > dirko::RTCIter< Key, Value, Hash, Equal >::operator++(int)
{
  RTCIter< Key, Value, Hash, Equal > ret = *this;
  ++id_;
  next();
  return ret;
}
template< class Key, class Value, class Hash, class Equal >
dirko::RTCIter< Key, Value, Hash, Equal > &dirko::RTCIter< Key, Value, Hash, Equal >::operator--()
{
  --id_;
  prev();
  return *this;
}
template< class Key, class Value, class Hash, class Equal >
dirko::RTCIter< Key, Value, Hash, Equal > dirko::RTCIter< Key, Value, Hash, Equal >::operator--(int)
{
  RTCIter< Key, Value, Hash, Equal > ret = *this;
  --id_;
  prev();
  return ret;
}
template< class Key, class Value, class Hash, class Equal >
void dirko::RTCIter< Key, Value, Hash, Equal >::next()
{
  while (id_ < data_->getSize() && !(*data_)[id_].occupied_) {
    ++id_;
  }
}

template< class Key, class Value, class Hash, class Equal >
void dirko::RTCIter< Key, Value, Hash, Equal >::prev()
{
  while (id_ > 1 && !(*data_)[id_].occupied_) {
    --id_;
  }
}
#endif
