#ifndef HASH_ITERS_HPP
#define HASH_ITERS_HPP

#include <top-it-vector.hpp>
#include <list.hpp>
#include <utility>

namespace afanasev
{
  template < class Key, class Value, class Hash, class Equal >
  class HashTable;

  template < class Key, class Value, class Hash, class Equal >
  class HashConstIter;

  template < class Key, class Value, class Hash, class Equal >
  class HashIter
  {
    friend class HashTable< Key, Value, Hash, Equal >;
    friend class HashConstIter< Key, Value, Hash, Equal >;
    using type = std::pair< Key, Value >;

    public:
    HashIter();
    HashIter(Vector< List< type > >* data, size_t capacity, size_t start_idx);

    HashIter & operator++();
    bool operator==(const HashIter & other) const;
    bool operator!=(const HashIter & other) const;
    type & operator*() const;

    private:
    Vector< List< type > > * data_;
    size_t capacity_;
    size_t idx_;
    LIter< type > listIt_;

    void findValid();
  };

  template < class Key, class Value, class Hash, class Equal >
  class HashConstIter
  {
    friend class HashTable< Key, Value, Hash, Equal >;
    friend class HashIter< Key, Value, Hash, Equal >;
    using type = std::pair< Key, Value >;

    public:
    HashConstIter();
    HashConstIter(const Vector< List< type > > * data, size_t capacity, size_t start_idx);

    HashConstIter& operator++();
    bool operator==(const HashConstIter & other) const;
    bool operator!=(const HashConstIter & other) const;
    const type & operator*() const;

    private:
    const Vector< List< type > > * data_;
    size_t capacity_;
    size_t idx_;
    LCIter< type > listIt_;

    void findValid();
  };
}

template < class Key, class Value, class Hash, class Equal >
afanasev::HashIter< Key, Value, Hash, Equal >::HashIter():
  data_(nullptr),
  capacity_(0),
  idx_(0),
  listIt_()
{}

template < class Key, class Value, class Hash, class Equal >
afanasev::HashIter< Key, Value, Hash, Equal >::HashIter(Vector< List< type > > * data,
  size_t capacity, size_t start_idx):
  data_(data),
  capacity_(capacity),
  idx_(start_idx),
  listIt_()
{
  findValid();
}

template < class Key, class Value, class Hash, class Equal >
void afanasev::HashIter< Key, Value, Hash, Equal >::findValid()
{
  while (idx_ < capacity_)
  {
    listIt_ = (*data_)[idx_].begin();
    if (listIt_ != LIter< type >())
    {
      return;
    }
    ++idx_;
  }

  data_ = nullptr;
}

template < class Key, class Value, class Hash, class Equal >
afanasev::HashIter< Key, Value, Hash, Equal > &
afanasev::HashIter< Key, Value, Hash, Equal >::operator++()
{
  ++listIt_;
  if (listIt_ != LIter< type >())
  {
    return *this;
  }

  ++idx_;
  findValid();
  return *this;
}

template < class Key, class Value, class Hash, class Equal >
bool afanasev::HashIter< Key, Value, Hash, Equal >::operator==(const HashIter & other) const
{
  if (!data_ && !other.data_)
  {
    return true;
  }

  if (!data_ || !other.data_)
  {
    return false;
  }
  return idx_ == other.idx_ && listIt_ == other.listIt_;
}

template < class Key, class Value, class Hash, class Equal >
bool afanasev::HashIter< Key, Value, Hash, Equal >::operator!=(const HashIter & other) const
{
  return !(*this == other);
}

template < class Key, class Value, class Hash, class Equal >
typename afanasev::HashIter< Key, Value, Hash, Equal >::type &
afanasev::HashIter< Key, Value, Hash, Equal >::operator*() const
{
  return *listIt_;
}

template < class Key, class Value, class Hash, class Equal >
afanasev::HashConstIter< Key, Value, Hash, Equal >::HashConstIter():
  data_(nullptr),
  capacity_(0),
  idx_(0),
  listIt_()
{}

template < class Key, class Value, class Hash, class Equal >
afanasev::HashConstIter< Key, Value, Hash, Equal >::HashConstIter(const Vector< List< type > > * data,
  size_t capacity, size_t start_idx):
  data_(data),
  capacity_(capacity),
  idx_(start_idx),
  listIt_()
{
  findValid();
}

template < class Key, class Value, class Hash, class Equal >
void afanasev::HashConstIter< Key, Value, Hash, Equal >::findValid()
{
  while (idx_ < capacity_)
  {
    listIt_ = (*data_)[idx_].begin();
    if (listIt_ != LCIter< type >())
    {
      return;
    }
    ++idx_;
  }
  data_ = nullptr;
}

template < class Key, class Value, class Hash, class Equal >
afanasev::HashConstIter< Key, Value, Hash, Equal > &
afanasev::HashConstIter< Key, Value, Hash, Equal >::operator++()
{
  ++listIt_;
  if (listIt_ != LCIter< type >())
  {
    return *this;
  }
  ++idx_;
  findValid();
  return *this;
}

template < class Key, class Value, class Hash, class Equal >
bool afanasev::HashConstIter< Key, Value, Hash, Equal >::operator==(const HashConstIter & other) const
{
  if (!data_ && !other.data_)
  {
    return true;
  }

  if (!data_ || !other.data_)
  {
    return false;
  }
  return idx_ == other.idx_ && listIt_ == other.listIt_;
}

template < class Key, class Value, class Hash, class Equal >
bool afanasev::HashConstIter< Key, Value, Hash, Equal >::operator!=(const HashConstIter & other) const
{
  return !(*this == other);
}

template < class Key, class Value, class Hash, class Equal >
const typename afanasev::HashConstIter< Key, Value, Hash, Equal >::type &
afanasev::HashConstIter< Key, Value, Hash, Equal >::operator*() const
{
  return *listIt_;
}

#endif
