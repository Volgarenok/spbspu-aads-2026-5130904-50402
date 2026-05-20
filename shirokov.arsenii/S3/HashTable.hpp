#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP
#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <utility>

namespace
{
  const size_t START_SIZE = 16;
}

namespace shirokov
{
  template< class Key, class Value, class Hash, class Equal >
  class HashTable;

  template< class Key, class Value, class Hash, class Equal >
  class HTIter
  {
  public:
    HTIter(HashTable< Key, Value, Hash, Equal >& ht, size_t pos);
    bool operator==(const HTIter&) const noexcept;
    bool operator!=(const HTIter&) const noexcept;
    HTIter& operator++();
    std::pair< Key, Value >& operator*();

  private:
    size_t pos_;
    HashTable< Key, Value, Hash, Equal >& table_;
  };

  template< class Key, class Value, class Hash, class Equal >
  class HTCIter
  {
  public:
    HTCIter(const HashTable< Key, Value, Hash, Equal >& ht, size_t pos);
    bool operator==(const HTCIter&) const noexcept;
    bool operator!=(const HTCIter&) const noexcept;
    HTCIter& operator++();
    const std::pair< Key, Value >& operator*();

  private:
    size_t pos_;
    const HashTable< Key, Value, Hash, Equal >& table_;
  };

  template< class Key, class Value, class Hash, class Equal >
  class HashTable
  {
  public:
    HashTable();
    HashTable(const HashTable& rhs);
    HashTable(HashTable&& rhs);
    ~HashTable();
    HashTable& operator=(const HashTable& rhs);
    HashTable& operator=(HashTable&& rhs);
    Value& operator[](Key key);

    HTIter< Key, Value, Hash, Equal > begin();
    HTCIter< Key, Value, Hash, Equal > begin() const;
    HTCIter< Key, Value, Hash, Equal > cbegin() const;

    HTIter< Key, Value, Hash, Equal > end();
    HTCIter< Key, Value, Hash, Equal > end() const;
    HTCIter< Key, Value, Hash, Equal > cend() const;

    bool insert(Key k, Value v);
    bool erase(Key k);
    bool contains(Key k) const;
    void rehash(size_t slots);
    bool empty() const;
    size_t slotsCount() const;
    size_t size() const;
    Value& at(Key key);
    const Value& at(Key key) const;
    void swap(HashTable< Key, Value, Hash, Equal >& rhs);

  private:
    friend class HTIter< Key, Value, Hash, Equal >;
    friend class HTCIter< Key, Value, Hash, Equal >;
    struct Slot
    {
      Key key;
      Value value;
      bool isEmpty = true;
    };
    size_t size_;
    size_t slotsCount_;
    Slot* slots_;
  };
}

template< class Key, class Value, class Hash, class Equal >
shirokov::HTIter< Key, Value, Hash, Equal > shirokov::HashTable< Key, Value, Hash, Equal >::begin()
{
  return HTIter< Key, Value, Hash, Equal >(*this, 0);
}

template< class Key, class Value, class Hash, class Equal >
shirokov::HTCIter< Key, Value, Hash, Equal > shirokov::HashTable< Key, Value, Hash, Equal >::cbegin() const
{
  return HTCIter< Key, Value, Hash, Equal >(*this, 0);
}

template< class Key, class Value, class Hash, class Equal >
shirokov::HTCIter< Key, Value, Hash, Equal > shirokov::HashTable< Key, Value, Hash, Equal >::begin() const
{
  return cbegin();
}

template< class Key, class Value, class Hash, class Equal >
std::pair< Key, Value >& shirokov::HTIter< Key, Value, Hash, Equal >::operator*()
{
  return {table_.slots_[pos_].key, table_.slots_[pos_].value};
}

template< class Key, class Value, class Hash, class Equal >
const std::pair< Key, Value >& shirokov::HTCIter< Key, Value, Hash, Equal >::operator*()
{
  return {table_.slots_[pos_].key, table_.slots_[pos_].value};
}

template< class Key, class Value, class Hash, class Equal >
shirokov::HTIter< Key, Value, Hash, Equal >& shirokov::HTIter< Key, Value, Hash, Equal >::operator++()
{
  while (pos_ < table_.size_ && table_.slots_[pos_].isEmpty)
  {
    ++pos_;
  }
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
shirokov::HTCIter< Key, Value, Hash, Equal >& shirokov::HTCIter< Key, Value, Hash, Equal >::operator++()
{
  while (pos_ < table_.size_ && table_.slots_[pos_].isEmpty)
  {
    ++pos_;
  }
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
bool shirokov::HTCIter< Key, Value, Hash, Equal >::operator!=(
    const HTCIter< Key, Value, Hash, Equal >& rhs) const noexcept
{
  return !(*this == rhs);
}

template< class Key, class Value, class Hash, class Equal >
bool shirokov::HTIter< Key, Value, Hash, Equal >::operator!=(
    const HTIter< Key, Value, Hash, Equal >& rhs) const noexcept
{
  return !(*this == rhs);
}

template< class Key, class Value, class Hash, class Equal >
bool shirokov::HTCIter< Key, Value, Hash, Equal >::operator==(
    const HTCIter< Key, Value, Hash, Equal >& rhs) const noexcept
{
  return (rhs.table_ == table_ && rhs.pos_ == pos_);
}

template< class Key, class Value, class Hash, class Equal >
bool shirokov::HTIter< Key, Value, Hash, Equal >::operator==(
    const HTIter< Key, Value, Hash, Equal >& rhs) const noexcept
{
  return (rhs.table_ == table_ && rhs.pos_ == pos_);
}

template< class Key, class Value, class Hash, class Equal >
shirokov::HTIter< Key, Value, Hash, Equal >::HTIter(HashTable< Key, Value, Hash, Equal >& ht, size_t pos):
  pos_(pos),
  table_(ht)
{}

template< class Key, class Value, class Hash, class Equal >
shirokov::HTCIter< Key, Value, Hash, Equal >::HTCIter(const HashTable< Key, Value, Hash, Equal >& ht, size_t pos):
  pos_(pos),
  table_(ht)
{}

template< class Key, class Value, class Hash, class Equal >
shirokov::HashTable< Key, Value, Hash, Equal >::HashTable():
  size_(START_SIZE),
  slotsCount_(0),
  slots_(new Slot[START_SIZE])
{}

template< class Key, class Value, class Hash, class Equal >
shirokov::HashTable< Key, Value, Hash, Equal >::HashTable::~HashTable()
{
  delete[] slots_;
}

template< class Key, class Value, class Hash, class Equal >
size_t shirokov::HashTable< Key, Value, Hash, Equal >::size() const
{
  return size_;
}

template< class Key, class Value, class Hash, class Equal >
size_t shirokov::HashTable< Key, Value, Hash, Equal >::slotsCount() const
{
  return slotsCount_;
}

template< class Key, class Value, class Hash, class Equal >
bool shirokov::HashTable< Key, Value, Hash, Equal >::empty() const
{
  return !slotsCount_;
}

template< class Key, class Value, class Hash, class Equal >
bool shirokov::HashTable< Key, Value, Hash, Equal >::insert(Key k, Value v)
{
  size_t limit = std::log2(size_);
  size_t pos = Hash{}(k) & (size_ - 1);
  for (size_t i = 0; i < limit; ++i)
  {
    if (slots_[pos].isEmpty)
    {
      slots_[pos] = {k, v, false};
      ++slotsCount_;
      return true;
    }
    if (slots_[pos].key == k)
    {
      return false;
    }
    pos = (pos + i) & (size_ - 1);
  }
  throw std::runtime_error("Hash table is full or cluster limit exceeded");
}

template< class Key, class Value, class Hash, class Equal >
bool shirokov::HashTable< Key, Value, Hash, Equal >::contains(Key k) const
{
  size_t limit = std::log2(size_);
  size_t pos = Hash{}(k) & (size_ - 1);
  for (size_t i = 0; i < limit; ++i)
  {
    if (!slots_[pos].isEmpty && slots_[pos].key == k)
    {
      return true;
    }
    pos = (pos + i) & (size_ - 1);
  }
  return false;
}

template< class Key, class Value, class Hash, class Equal >
const Value& shirokov::HashTable< Key, Value, Hash, Equal >::at(Key k) const
{
  size_t limit = std::log2(size_);
  size_t pos = Hash{}(k) & (size_ - 1);
  for (size_t i = 0; i < limit; ++i)
  {
    if (!slots_[pos].isEmpty && slots_[pos].key == k)
    {
      return slots_[pos].value;
    }
    pos = (pos + i) & (size_ - 1);
  }
  throw std::out_of_range("Value with this key not found");
}

template< class Key, class Value, class Hash, class Equal >
Value& shirokov::HashTable< Key, Value, Hash, Equal >::at(Key k)
{
  const HashTable< Key, Value, Hash, Equal >* cthis = this;
  const Value& ret = cthis->at(k);
  return const_cast< Value& >(ret);
}

template< class Key, class Value, class Hash, class Equal >
Value& shirokov::HashTable< Key, Value, Hash, Equal >::operator[](Key k)
{
  size_t limit = std::log2(size_);
  size_t pos = Hash{}(k) & (size_ - 1);
  for (size_t i = 0; i < limit; ++i)
  {
    if (!slots_[pos].isEmpty && slots_[pos].key == k)
    {
      return slots_[pos].value;
    }
    pos = (pos + i) & (size_ - 1);
  }

  pos = Hash{}(k) & (size_ - 1);
  for (size_t i = 0; i < limit; ++i)
  {
    if (slots_[pos].isEmpty)
    {
      slots_[pos] = {k, Value(), false};
      ++slotsCount_;
      return slots_[pos].value;
    }
    pos = (pos + i) & (size_ - 1);
  }
  throw std::runtime_error("Hash table is full or cluster limit exceeded");
}

template< class Key, class Value, class Hash, class Equal >
bool shirokov::HashTable< Key, Value, Hash, Equal >::erase(Key k)
{
  size_t limit = std::log2(size_);
  size_t pos = Hash{}(k) & (size_ - 1);
  for (size_t i = 0; i < limit; ++i)
  {
    if (!slots_[pos].isEmpty && slots_[pos].key == k)
    {
      slots_[pos].isEmpty = true;
      return true;
    }
    pos = (pos + i) & (size_ - 1);
  }
  return false;
}

template< class Key, class Value, class Hash, class Equal >
shirokov::HashTable< Key, Value, Hash, Equal >::HashTable(const HashTable& rhs):
  size_(rhs.size_),
  slotsCount_(rhs.slotsCount_),
  slots_(new Slot[rhs.size_])
{
  for (size_t i = 0; i < size_; ++i)
  {
    slots_[i] = rhs.slots_[i];
  }
}

template< class Key, class Value, class Hash, class Equal >
shirokov::HashTable< Key, Value, Hash, Equal >::HashTable(HashTable&& rhs):
  size_(rhs.size_),
  slotsCount_(rhs.slotsCount_),
  slots_(rhs.slots_)
{
  rhs.slots_ = nullptr;
  rhs.size_ = 0;
  rhs.slotsCount_ = 0;
}

template< class Key, class Value, class Hash, class Equal >
void shirokov::HashTable< Key, Value, Hash, Equal >::swap(HashTable< Key, Value, Hash, Equal >& rhs)
{
  std::swap(size_, rhs.size_);
  std::swap(slotsCount_, rhs.slotsCount_);
  std::swap(slots_, rhs.slots_);
}

template< class Key, class Value, class Hash, class Equal >
shirokov::HashTable< Key, Value, Hash, Equal >&
shirokov::HashTable< Key, Value, Hash, Equal >::operator=(const HashTable& rhs)
{
  if (this == &rhs)
  {
    return *this;
  }
  shirokov::HashTable< Key, Value, Hash, Equal > cpy(rhs);
  swap(cpy);
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
shirokov::HashTable< Key, Value, Hash, Equal >&
shirokov::HashTable< Key, Value, Hash, Equal >::operator=(HashTable&& rhs)
{
  if (this == &rhs)
  {
    return *this;
  }
  swap(rhs);
  return *this;
}

#endif
