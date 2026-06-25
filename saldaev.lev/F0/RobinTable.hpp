#ifndef ROBINTABLE_HPP
#define ROBINTABLE_HPP
#include <vector.hpp>

namespace saldaev
{
  template< class Key, class Value, class Hash, class Equal >
  class HashTable
  {
    struct Node
    {
      bool taken_;
      std::pair< Key, Value > key_value_;
      size_t dist_;

      Node();
    };

    Vector< Node > data_;
    Hash hasher_;
    Equal key_eq_;
    size_t slots_;
    size_t elements_;
    float max_load_factor_;

  public:
    struct Iterator
    {
      friend class HashTable;

    public:
      std::pair< Key, Value > &operator*();
      std::pair< Key, Value > *operator->();

      Iterator &operator++();
      Iterator operator++(int);

      bool operator==(const Iterator &other) const;
      bool operator!=(const Iterator &other) const;

    private:
      typename Vector< Node >::Iterator it_;
      Vector< Node > *v_;

      Iterator(typename Vector< Node >::Iterator it, Vector< Node > *v);
    };

    struct ConstIterator
    {
      friend class HashTable;

    public:
      const std::pair< Key, Value > &operator*() const;
      const std::pair< Key, Value > *operator->() const;

      ConstIterator &operator++();
      ConstIterator operator++(int);

      bool operator==(const ConstIterator &other) const;
      bool operator!=(const ConstIterator &other) const;

    private:
      typename Vector< Node >::ConstIterator it_;
      const Vector< Node > *v_;

      ConstIterator(typename Vector< Node >::ConstIterator it, const Vector< Node > *v);
    };

    HashTable(size_t slots = 11, float load_factor = 0.75f);

    void add(Key k, Value v);
    bool has(Key k) const noexcept;
    Value get(Key k) const;
    Value &at(Key k);
    void remove(Key k);
    void rewrite(Key k, Value v);

    void rehash(size_t slots);
    void clear() noexcept;
    size_t size() const noexcept;
    float load_factor() const noexcept;
    bool empty() const noexcept;

    Iterator begin();
    Iterator end();
    ConstIterator begin() const;
    ConstIterator end() const;
  };
}

template< class Key, class Value, class Hash, class Equal >
saldaev::HashTable< Key, Value, Hash, Equal >::Node::Node():
  taken_(false),
  key_value_({Key(), Value()}),
  dist_(0)
{}

template< class Key, class Value, class Hash, class Equal >
saldaev::HashTable< Key, Value, Hash, Equal >::HashTable(size_t slots, float load_factor):
  data_(slots),
  slots_(slots),
  elements_(0),
  max_load_factor_(load_factor)
{
  for (size_t i = 0; i < slots; ++i) {
    data_.pushBack(Node());
  }
}

template< class Key, class Value, class Hash, class Equal >
void saldaev::HashTable< Key, Value, Hash, Equal >::add(Key k, Value v)
{
  if (has(k)) {
    throw std::invalid_argument("Key already exists");
  }

  size_t curr_idx = hasher_(k) % slots_;
  size_t dist = 0;
  while (data_[curr_idx].taken_) {
    if (dist > data_[curr_idx].dist_) {
      std::swap(dist, data_[curr_idx].dist_);
      std::swap(k, data_[curr_idx].key_value_.first);
      std::swap(v, data_[curr_idx].key_value_.second);
    }
    curr_idx = (curr_idx + 1) % slots_;
    ++dist;
  }
  data_[curr_idx].taken_ = true;
  data_[curr_idx].dist_ = dist;
  data_[curr_idx].key_value_.first = k;
  data_[curr_idx].key_value_.second = v;

  ++elements_;
  if (max_load_factor_ * slots_ <= elements_) {
    rehash(slots_ * 2);
  }
}

template< class Key, class Value, class Hash, class Equal >
bool saldaev::HashTable< Key, Value, Hash, Equal >::has(Key k) const noexcept
{
  size_t curr_idx = hasher_(k) % slots_;
  size_t dist = 0;
  while (data_[curr_idx].taken_ && (dist <= data_[curr_idx].dist_)) {
    if (key_eq_(k, data_[curr_idx].key_value_.first)) {
      return true;
    }
    curr_idx = (curr_idx + 1) % slots_;
    ++dist;
  }
  return false;
}

template< class Key, class Value, class Hash, class Equal >
Value saldaev::HashTable< Key, Value, Hash, Equal >::get(Key k) const
{
  size_t curr_idx = hasher_(k) % slots_;
  size_t dist = 0;
  while (data_[curr_idx].taken_ && (dist <= data_[curr_idx].dist_)) {
    if (key_eq_(k, data_[curr_idx].key_value_.first)) {
      return data_[curr_idx].key_value_.second;
    }
    curr_idx = (curr_idx + 1) % slots_;
    ++dist;
  }
  throw std::invalid_argument("Key does not exist");
}

template< class Key, class Value, class Hash, class Equal >
Value &saldaev::HashTable< Key, Value, Hash, Equal >::at(Key k)
{
  size_t curr_idx = hasher_(k) % slots_;
  size_t dist = 0;
  while (dist <= data_[curr_idx].dist_) {
    if (key_eq_(k, data_[curr_idx].key_value_.first)) {
      return data_[curr_idx].key_value_.second;
    }
    curr_idx = (curr_idx + 1) % slots_;
    ++dist;
  }
  throw std::invalid_argument("Key does not exist");
}

template< class Key, class Value, class Hash, class Equal >
void saldaev::HashTable< Key, Value, Hash, Equal >::remove(Key k)
{
  size_t curr_idx = hasher_(k) % slots_;
  size_t dist = 0;
  while (data_[curr_idx].taken_ && (dist <= data_[curr_idx].dist_)) {
    if (key_eq_(k, data_[curr_idx].key_value_.first)) {
      while (data_[(curr_idx + 1) % slots_].taken_ && data_[(curr_idx + 1) % slots_].dist_ > 0) {
        data_[curr_idx] = std::move(data_[(curr_idx + 1) % slots_]);
        data_[curr_idx].dist_--;
        curr_idx = (curr_idx + 1) % slots_;
      }
      data_[curr_idx].taken_ = false;
      data_[curr_idx].dist_ = 0;
      --elements_;
      return;
    }

    curr_idx = (curr_idx + 1) % slots_;
    ++dist;
  }
  throw std::invalid_argument("Key does not exist");
}

template< class Key, class Value, class Hash, class Equal >
void saldaev::HashTable< Key, Value, Hash, Equal >::rewrite(Key k, Value v)
{
  size_t curr_idx = hasher_(k) % slots_;
  size_t dist = 0;
  while (data_[curr_idx].taken_ && (dist <= data_[curr_idx].dist_)) {
    if (key_eq_(k, data_[curr_idx].key_value_.first)) {
      data_[curr_idx].key_value_.second = v;
      return;
    }
    curr_idx = (curr_idx + 1) % slots_;
    ++dist;
  }
  throw std::invalid_argument("Key does not exist");
}

template< class Key, class Value, class Hash, class Equal >
void saldaev::HashTable< Key, Value, Hash, Equal >::rehash(size_t slots)
{
  HashTable newOne(slots);

  for (size_t i = 0; i < slots_; ++i) {
    if (data_[i].taken_) {
      newOne.add(data_[i].key_value_.first, data_[i].key_value_.second);
    }
  }

  std::swap(*this, newOne);
}

template< class Key, class Value, class Hash, class Equal >
void saldaev::HashTable< Key, Value, Hash, Equal >::clear() noexcept
{
  for (size_t i = 0; i < slots_; ++i) {
    data_[i].taken_ = false;
  }
  elements_ = 0;
}

template< class Key, class Value, class Hash, class Equal >
size_t saldaev::HashTable< Key, Value, Hash, Equal >::size() const noexcept
{
  return elements_;
}

template< class Key, class Value, class Hash, class Equal >
float saldaev::HashTable< Key, Value, Hash, Equal >::load_factor() const noexcept
{
  return static_cast< float >(elements_) / slots_;
}

template< class Key, class Value, class Hash, class Equal >
bool saldaev::HashTable< Key, Value, Hash, Equal >::empty() const noexcept
{
  return elements_ == 0;
}

template< class Key, class Value, class Hash, class Equal >
typename saldaev::HashTable< Key, Value, Hash, Equal >::Iterator saldaev::HashTable< Key, Value, Hash, Equal >::begin()
{
  auto it = data_.begin();
  while (it != data_.end() && !(it->taken_)) {
    it++;
  }
  return Iterator(it, &data_);
}

template< class Key, class Value, class Hash, class Equal >
typename saldaev::HashTable< Key, Value, Hash, Equal >::Iterator saldaev::HashTable< Key, Value, Hash, Equal >::end()
{
  return Iterator(data_.end(), &data_);
}

template< class Key, class Value, class Hash, class Equal >
typename saldaev::HashTable< Key, Value, Hash, Equal >::ConstIterator
saldaev::HashTable< Key, Value, Hash, Equal >::begin() const
{
  auto it = data_.begin();
  while (it != data_.end() && !(it->taken_)) {
    it++;
  }
  return ConstIterator(it, &data_);
}

template< class Key, class Value, class Hash, class Equal >
typename saldaev::HashTable< Key, Value, Hash, Equal >::ConstIterator
saldaev::HashTable< Key, Value, Hash, Equal >::end() const
{
  return ConstIterator(data_.end(), &data_);
}

template< class Key, class Value, class Hash, class Equal >
saldaev::HashTable< Key, Value, Hash, Equal >::Iterator::Iterator(typename Vector< Node >::Iterator it,
                                                                  Vector< Node > *v):
  it_(it),
  v_(v)
{}

template< class Key, class Value, class Hash, class Equal >
std::pair< Key, Value > &saldaev::HashTable< Key, Value, Hash, Equal >::Iterator::operator*()
{
  return it_->key_value_;
}

template< class Key, class Value, class Hash, class Equal >
std::pair< Key, Value > *saldaev::HashTable< Key, Value, Hash, Equal >::Iterator::operator->()
{
  return &(it_->key_value_);
}

template< class Key, class Value, class Hash, class Equal >
typename saldaev::HashTable< Key, Value, Hash, Equal >::Iterator::Iterator &
saldaev::HashTable< Key, Value, Hash, Equal >::Iterator::operator++()
{
  it_++;
  while (it_ != v_->end() && !(it_->taken_)) {
    it_++;
  }
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
typename saldaev::HashTable< Key, Value, Hash, Equal >::Iterator::Iterator
saldaev::HashTable< Key, Value, Hash, Equal >::Iterator::operator++(int)
{
  auto cp = *this;
  it_++;
  while (it_ != v_->end() && !(it_->taken_)) {
    it_++;
  }
  return cp;
}

template< class Key, class Value, class Hash, class Equal >
bool saldaev::HashTable< Key, Value, Hash, Equal >::Iterator::operator==(const Iterator &other) const
{
  return it_ == other.it_;
}

template< class Key, class Value, class Hash, class Equal >
bool saldaev::HashTable< Key, Value, Hash, Equal >::Iterator::operator!=(const Iterator &other) const
{
  return !(*this == other);
}

template< class Key, class Value, class Hash, class Equal >
saldaev::HashTable< Key, Value, Hash, Equal >::ConstIterator::ConstIterator(typename Vector< Node >::ConstIterator it,
                                                                            const Vector< Node > *v):
  it_(it),
  v_(v)
{}

template< class Key, class Value, class Hash, class Equal >
const std::pair< Key, Value > &saldaev::HashTable< Key, Value, Hash, Equal >::ConstIterator::operator*() const
{
  return it_->key_value_;
}

template< class Key, class Value, class Hash, class Equal >
const std::pair< Key, Value > *saldaev::HashTable< Key, Value, Hash, Equal >::ConstIterator::operator->() const
{
  return &(it_->key_value_);
}

template< class Key, class Value, class Hash, class Equal >
typename saldaev::HashTable< Key, Value, Hash, Equal >::ConstIterator::ConstIterator &
saldaev::HashTable< Key, Value, Hash, Equal >::ConstIterator::operator++()
{
  it_++;
  while (it_ != v_->end() && !(it_->taken_)) {
    it_++;
  }
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
typename saldaev::HashTable< Key, Value, Hash, Equal >::ConstIterator::ConstIterator
saldaev::HashTable< Key, Value, Hash, Equal >::ConstIterator::operator++(int)
{
  auto cp = *this;
  it_++;
  while (it_ != v_->end() && !(it_->taken_)) {
    it_++;
  }
  return cp;
}

template< class Key, class Value, class Hash, class Equal >
bool saldaev::HashTable< Key, Value, Hash, Equal >::ConstIterator::operator==(const ConstIterator &other) const
{
  return it_ == other.it_;
}

template< class Key, class Value, class Hash, class Equal >
bool saldaev::HashTable< Key, Value, Hash, Equal >::ConstIterator::operator!=(const ConstIterator &other) const
{
  return !(*this == other);
}

#endif
