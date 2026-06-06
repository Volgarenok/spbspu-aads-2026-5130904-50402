#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP
#include <cstddef>
#include <utility>
#include "../common/List.hpp"
#include "../common/vector.hpp"

namespace saldaev
{
  template< class Key, class Value, class Hash, class Equal >
  struct HashTable
  {
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
      Vector< List< std::pair< Key, Value > > > *buckets_;
      size_t bucket_idx_;
      typename List< std::pair< Key, Value > >::LIter list_iter_;

      Iterator(Vector< List< std::pair< Key, Value > > > *b, size_t idx,
               typename List< std::pair< Key, Value > >::LIter it);
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
      const Vector< List< std::pair< Key, Value > > > *buckets_;
      size_t bucket_idx_;
      typename List< std::pair< Key, Value > >::LCIter list_iter_;

      ConstIterator(const Vector< List< std::pair< Key, Value > > > *b, size_t idx,
                    typename List< std::pair< Key, Value > >::LCIter it);
    };

    HashTable(size_t slots, Hash hasher, Equal key_eq);
    ~HashTable() = default;

    void add(Key k, Value v);
    bool has(Key k) const noexcept;
    Value get(Key k) const;
    Value &at(Key k);
    void remove(Key k);
    void rewrite(Key k, Value v);

    void rehash(size_t slots);
    void clear() noexcept;
    size_t size() const noexcept;
    size_t bucket_count() const noexcept;
    bool empty() const noexcept;
    void swap(HashTable &other) noexcept;

    Iterator begin();
    Iterator end();
    ConstIterator begin() const;
    ConstIterator end() const;

  private:
    Vector< List< std::pair< Key, Value > > > data_;
    Hash hasher_;
    Equal key_eq_;
    size_t slots_ = 0;
    size_t elements_ = 0;
  };
}

template< class Key, class Value, class Hash, class Equal >
saldaev::HashTable< Key, Value, Hash, Equal >::HashTable(size_t slots, Hash hasher, Equal key_eq):
  data_(Vector< List< std::pair< Key, Value > > >(slots)),
  hasher_(hasher),
  key_eq_(key_eq),
  slots_(slots),
  elements_(0)
{
  for (size_t i = 0; i < slots; ++i) {
    data_.pushBack(List< std::pair< Key, Value > >());
  }
}

template< class Key, class Value, class Hash, class Equal >
void saldaev::HashTable< Key, Value, Hash, Equal >::add(Key k, Value v)
{
  if (has(k)) {
    throw std::invalid_argument("Key already exists");
  }

  data_[hasher_(k) % slots_].pushBack({k, v});
  ++elements_;
}

template< class Key, class Value, class Hash, class Equal >
bool saldaev::HashTable< Key, Value, Hash, Equal >::has(Key k) const noexcept
{
  auto it = data_[hasher_(k) % slots_].begin();
  while (it != data_[hasher_(k) % slots_].end()) {
    if (key_eq_(it->first, k)) {
      return true;
    }
    ++it;
  }
  return false;
}

template< class Key, class Value, class Hash, class Equal >
Value saldaev::HashTable< Key, Value, Hash, Equal >::get(Key k) const
{
  auto it = data_[hasher_(k) % slots_].begin();
  while (it != data_[hasher_(k) % slots_].end()) {
    if (key_eq_(it->first, k)) {
      return it->second;
    }
    ++it;
  }
  throw std::invalid_argument("Key does not exist");
}

template< class Key, class Value, class Hash, class Equal >
Value &saldaev::HashTable< Key, Value, Hash, Equal >::at(Key k)
{
  auto it = data_[hasher_(k) % slots_].begin();
  while (it != data_[hasher_(k) % slots_].end()) {
    if (key_eq_(it->first, k)) {
      return it->second;
    }
    ++it;
  }
  throw std::invalid_argument("Key does not exist");
}

template< class Key, class Value, class Hash, class Equal >
void saldaev::HashTable< Key, Value, Hash, Equal >::remove(Key k)
{
  size_t idx = hasher_(k) % slots_;
  auto it = data_[idx].begin();
  while (it != data_[idx].end()) {
    if (key_eq_(it->first, k)) {
      data_[idx].erase(it);
      --elements_;
      return;
    }
    ++it;
  }
  throw std::invalid_argument("Key does not exist");
}

template< class Key, class Value, class Hash, class Equal >
void saldaev::HashTable< Key, Value, Hash, Equal >::rewrite(Key k, Value v)
{
  size_t idx = hasher_(k) % slots_;
  auto it = data_[idx].begin();
  while (it != data_[idx].end()) {
    if (key_eq_(it->first, k)) {
      it->second = v;
      return;
    }
    ++it;
  }
  throw std::invalid_argument("Key does not exist");
}

template< class Key, class Value, class Hash, class Equal >
void saldaev::HashTable< Key, Value, Hash, Equal >::rehash(size_t slots)
{
  Vector< List< std::pair< Key, Value > > > newData(slots);
  for (size_t i = 0; i < slots; ++i) {
    newData.pushBack(List< std::pair< Key, Value > >());
  }

  auto it = this->begin();
  while (it != this->end()) {
    newData[hasher_(it->first) % slots].pushBack({std::move(it->first), std::move(it->second)});
    ++it;
  }

  data_.swap(newData);
  slots_ = slots;
}

template< class Key, class Value, class Hash, class Equal >
void saldaev::HashTable< Key, Value, Hash, Equal >::clear() noexcept
{
  auto it = data_.begin();
  while (it != data_.end()) {
    it->clear();
    ++it;
  }
  elements_ = 0;
}

template< class Key, class Value, class Hash, class Equal >
size_t saldaev::HashTable< Key, Value, Hash, Equal >::size() const noexcept
{
  return elements_;
}

template< class Key, class Value, class Hash, class Equal >
size_t saldaev::HashTable< Key, Value, Hash, Equal >::bucket_count() const noexcept
{
  return slots_;
}

template< class Key, class Value, class Hash, class Equal >
bool saldaev::HashTable< Key, Value, Hash, Equal >::empty() const noexcept
{
  return !elements_;
}

template< class Key, class Value, class Hash, class Equal >
void saldaev::HashTable< Key, Value, Hash, Equal >::swap(HashTable &other) noexcept
{
  data_.swap(other.data_);
  std::swap(hasher_, other.hasher_);
  std::swap(key_eq_, other.key_eq_);
  std::swap(slots_, other.slots_);
  std::swap(elements_, other.elements_);
}

template< class Key, class Value, class Hash, class Equal >
typename saldaev::HashTable< Key, Value, Hash, Equal >::Iterator saldaev::HashTable< Key, Value, Hash, Equal >::begin()
{
  if (slots_ == 0) {
    return Iterator(&data_, 0, typename List< std::pair< Key, Value > >::LIter());
  }

  size_t idx = 0;
  typename List< std::pair< Key, Value > >::LIter it = data_[0].begin();
  while (it == data_[idx].end()) {
    ++idx;
    if (idx == data_.getSize()) {
      return Iterator(&data_, idx, typename List< std::pair< Key, Value > >::LIter());
    }
    it = data_[idx].begin();
  }
  return Iterator(&data_, idx, it);
}

template< class Key, class Value, class Hash, class Equal >
typename saldaev::HashTable< Key, Value, Hash, Equal >::Iterator saldaev::HashTable< Key, Value, Hash, Equal >::end()
{
  return Iterator(&data_, slots_, typename List< std::pair< Key, Value > >::LIter());
}

template< class Key, class Value, class Hash, class Equal >
typename saldaev::HashTable< Key, Value, Hash, Equal >::ConstIterator
saldaev::HashTable< Key, Value, Hash, Equal >::begin() const
{
  if (slots_ == 0) {
    return ConstIterator(&data_, 0, typename List< std::pair< Key, Value > >::LCIter());
  }

  size_t idx = 0;
  typename List< std::pair< Key, Value > >::LCIter it = data_[0].begin();
  while (it == data_[idx].end()) {
    ++idx;
    if (idx == data_.getSize()) {
      return ConstIterator(&data_, idx, typename List< std::pair< Key, Value > >::LCIter());
    }
    it = data_[idx].begin();
  }
  return ConstIterator(&data_, idx, it);
}

template< class Key, class Value, class Hash, class Equal >
typename saldaev::HashTable< Key, Value, Hash, Equal >::ConstIterator
saldaev::HashTable< Key, Value, Hash, Equal >::end() const
{
  return ConstIterator(&data_, slots_, typename List< std::pair< Key, Value > >::LCIter());
}

template< class Key, class Value, class Hash, class Equal >
saldaev::HashTable< Key, Value, Hash, Equal >::Iterator::Iterator(Vector< List< std::pair< Key, Value > > > *b,
                                                                  size_t idx,
                                                                  typename List< std::pair< Key, Value > >::LIter it):
  buckets_(b),
  bucket_idx_(idx),
  list_iter_(it)
{}

template< class Key, class Value, class Hash, class Equal >
std::pair< Key, Value > &saldaev::HashTable< Key, Value, Hash, Equal >::Iterator::operator*()
{
  return *list_iter_;
}

template< class Key, class Value, class Hash, class Equal >
std::pair< Key, Value > *saldaev::HashTable< Key, Value, Hash, Equal >::Iterator::operator->()
{
  return &(*list_iter_);
}

template< class Key, class Value, class Hash, class Equal >
typename saldaev::HashTable< Key, Value, Hash, Equal >::Iterator &
saldaev::HashTable< Key, Value, Hash, Equal >::Iterator::operator++()
{
  ++list_iter_;
  while (list_iter_ == (*buckets_)[bucket_idx_].end()) {
    ++bucket_idx_;
    if (bucket_idx_ == buckets_->getSize()) {
      list_iter_ = typename List< std::pair< Key, Value > >::LIter();
      return *this;
    }
    list_iter_ = (*buckets_)[bucket_idx_].begin();
  }
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
typename saldaev::HashTable< Key, Value, Hash, Equal >::Iterator
saldaev::HashTable< Key, Value, Hash, Equal >::Iterator::operator++(int)
{
  Iterator ret = *this;
  ++list_iter_;
  while (list_iter_ == (*buckets_)[bucket_idx_].end()) {
    ++bucket_idx_;
    if (bucket_idx_ == buckets_->getSize()) {
      list_iter_ = typename List< std::pair< Key, Value > >::LIter();
      return ret;
    }
    list_iter_ = (*buckets_)[bucket_idx_].begin();
  }
  return ret;
}

template< class Key, class Value, class Hash, class Equal >
bool saldaev::HashTable< Key, Value, Hash, Equal >::Iterator::operator==(const Iterator &other) const
{
  return (buckets_ == other.buckets_) && (bucket_idx_ == other.bucket_idx_) && (list_iter_ == other.list_iter_);
}

template< class Key, class Value, class Hash, class Equal >
bool saldaev::HashTable< Key, Value, Hash, Equal >::Iterator::operator!=(const Iterator &other) const
{
  return !(*this == other);
}

template< class Key, class Value, class Hash, class Equal >
saldaev::HashTable< Key, Value, Hash, Equal >::ConstIterator::ConstIterator(
    const Vector< List< std::pair< Key, Value > > > *b, size_t idx,
    typename List< std::pair< Key, Value > >::LCIter it):
  buckets_(b),
  bucket_idx_(idx),
  list_iter_(it)
{}

template< class Key, class Value, class Hash, class Equal >
const std::pair< Key, Value > &saldaev::HashTable< Key, Value, Hash, Equal >::ConstIterator::operator*() const
{
  return *list_iter_;
}

template< class Key, class Value, class Hash, class Equal >
const std::pair< Key, Value > *saldaev::HashTable< Key, Value, Hash, Equal >::ConstIterator::operator->() const
{
  return &(*list_iter_);
}

template< class Key, class Value, class Hash, class Equal >
typename saldaev::HashTable< Key, Value, Hash, Equal >::ConstIterator &
saldaev::HashTable< Key, Value, Hash, Equal >::ConstIterator::operator++()
{
  ++list_iter_;
  while (list_iter_ == (*buckets_)[bucket_idx_].end()) {
    ++bucket_idx_;
    if (bucket_idx_ == buckets_->getSize()) {
      list_iter_ = typename List< std::pair< Key, Value > >::LCIter();
      return *this;
    }
    list_iter_ = (*buckets_)[bucket_idx_].begin();
  }
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
typename saldaev::HashTable< Key, Value, Hash, Equal >::ConstIterator
saldaev::HashTable< Key, Value, Hash, Equal >::ConstIterator::operator++(int)
{
  ConstIterator ret = *this;
  ++list_iter_;
  while (list_iter_ == (*buckets_)[bucket_idx_].end()) {
    ++bucket_idx_;
    if (bucket_idx_ == buckets_->getSize()) {
      list_iter_ = typename List< std::pair< Key, Value > >::LCIter();
      return ret;
    }
    list_iter_ = (*buckets_)[bucket_idx_].begin();
  }
  return ret;
}

template< class Key, class Value, class Hash, class Equal >
bool saldaev::HashTable< Key, Value, Hash, Equal >::ConstIterator::operator==(const ConstIterator &other) const
{
  return (buckets_ == other.buckets_) && (bucket_idx_ == other.bucket_idx_) && (list_iter_ == other.list_iter_);
}

template< class Key, class Value, class Hash, class Equal >
bool saldaev::HashTable< Key, Value, Hash, Equal >::ConstIterator::operator!=(const ConstIterator &other) const
{
  return !(*this == other);
}

#endif
