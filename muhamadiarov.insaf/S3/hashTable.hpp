#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP
#include <stdexcept>
#include <utility>
#include <memory>

namespace muhamadiarov
{
  enum class State
  {
    EMPTY,
    OCCUPIED,
    DELETE
  };

  template < class Key, class Value >
  struct Slot
  {
    Key key_;
    Value value_;
    State state_ = State::EMPTY;
    Slot() = default;
    Slot(const Key& key, const Value& value);
  };

  template < class Key, class Value, class Hash, class Equal >
  class HTIter;

  template < class Key, class Value, class Hash, class Equal >
  class HTCIter;

  template < class Key, class Value, class Hash, class Equal >
  class HashTable
  {
  public:
    HashTable(size_t bucketCapacity, size_t bucketCount);
    HashTable(const HashTable& other);
    HashTable(HashTable&& other) noexcept;
    HashTable& operator=(const HashTable& other);
    HashTable& operator=(HashTable&& other) noexcept;
    ~HashTable() noexcept;


    void add(const Key& k, const Value& v);
    Value drop(const Key& k);
    bool has(const Key& k) const noexcept;
    void rehash(size_t countBucket, size_t newBucketCapacity);
    size_t size() const noexcept;
    size_t capacity() const noexcept;
    size_t bucketCapacity() const noexcept;
    size_t bucketCount() const noexcept;
    Value& get(const Key& k);
    const Value& cget(const Key& k);
    void clear() noexcept;

    HTIter< Key, Value, Hash, Equal > begin();
    HTIter< Key, Value, Hash, Equal > end();
    HTCIter< Key, Value, Hash, Equal > cbegin() const;
    HTCIter< Key, Value, Hash, Equal > cend() const;
  private:
    friend class HTIter< Key, Value, Hash, Equal >;
    friend class HTCIter< Key, Value, Hash, Equal >;
    size_t bucketCapacity_;
    size_t bucketCount_;
    size_t otherflowBucketIndex_;
    size_t capacity_;
    size_t size_;
    Slot< Key, Value >* slots_;
    Hash hash_;
    Equal equal_;
    size_t getBucketStart(const Key& k) const;
    bool findSlot(const Key& k, size_t& id) const;
    bool findFreeSlot(const Key& k, size_t& id) const;
  };

  template < class Key, class Value, class Hash, class Equal >
  class HTIter
  {
  public:
    HTIter(HashTable< Key, Value, Hash, Equal >* ht, size_t index = 0);

    Slot< Key, Value >& operator*() const;
    Slot< Key, Value >* operator->();
    bool operator==(const HTIter& other) const;
    bool operator!=(const HTIter& other) const;

    HTIter& operator++();
    HTIter operator++(int);
  private:
    HashTable< Key, Value, Hash, Equal >* ht_;
    size_t currentIndex_;
    void advanceToNextOccupied();
  };

  template < class Key, class Value, class Hash, class Equal >
  class HTCIter
  {
  public:
    HTCIter(const HashTable< Key, Value, Hash, Equal >* ht, size_t index = 0);

    const Slot< Key, Value >& operator*() const;
    const Slot< Key, Value >* operator->() const;
    bool operator==(const HTCIter& other) const;
    bool operator!=(const HTCIter& other) const;

    HTCIter& operator++();
    HTCIter operator++(int);
  private:
    const HashTable< Key, Value, Hash, Equal >* ht_;
    size_t currentIndex_;
    void advanceToNextOccupied();
  };
}

namespace muh = muhamadiarov;

template < class K, class V >
muh::Slot< K, V >::Slot(const K& key, const V& value):
  key_(key),
  value_(value),
state_(State::OCCUPIED)
{}

template < class K, class V, class H, class E >
muh::HashTable< K, V, H, E >::HashTable(size_t bucketCapacity, size_t bucketCount):
  bucketCapacity_(bucketCapacity),
  bucketCount_(bucketCount),
  otherflowBucketIndex_(bucketCapacity * bucketCount),
  capacity_((bucketCount + 1) * bucketCapacity),
  size_(0)
{
  if (bucketCapacity == 0 || bucketCount == 0)
  {
    throw std::invalid_argument("Count of buckets and their capacity must be > 0");
  }
  slots_ = new Slot< K, V >[capacity_];
  for (size_t i = 0; i < capacity_; ++i)
  {
    slots_[i].state_ = State::EMPTY;
  }
}

template < class K, class V, class H, class E >
muh::HashTable< K, V, H, E >::HashTable(const HashTable& other):
  bucketCapacity_(other.bucketCapacity_),
  bucketCount_(other.bucketCount_),
  otherflowBucketIndex_(other.otherflowBucketIndex_),
  capacity_(other.capacity_),
  size_(other.size_),
  hash_(other.hash_),
  equal_(other.equal_)
{
  slots_ = new Slot< K, V >[capacity_];
  try
  {
    for (size_t i = 0; i < capacity_; ++i)
    {
      if (other.slots_[i].state_ != State::EMPTY)
      {
        slots_[i] = other.slots_[i];
      }
    }
  }
  catch(...)
  {
    delete [] slots_;
    throw;
  }
}

template < class K, class V, class H, class E >
muh::HashTable< K, V, H, E >::HashTable(HashTable&& other) noexcept:
  bucketCapacity_(other.bucketCapacity_),
  bucketCount_(other.bucketCount_),
  otherflowBucketIndex_(other.otherflowBucketIndex_),
  capacity_(other.capacity_),
  size_(other.size_),
  slots_(other.slots_),
  hash_(std::move(other.hash_)),
  equal_(std::move(other.equal_))
{
  other.slots_ = nullptr;
  other.bucketCapacity_ = 0;
  other.bucketCount_ = 0;
  other.otherflowBucketIndex_ = 0;
  other.capacity_ = 0;
  other.size_ = 0;
}

template < class K, class V, class H, class E >
muh::HashTable< K, V, H, E >& muh::HashTable< K, V, H, E >::operator=(const HashTable& other)
{
  if (this != &other) {
    delete[] slots_;

    bucketCapacity_ = other.bucketCapacity_;
    bucketCount_ = other.bucketCount_;
    otherflowBucketIndex_ = other.otherflowBucketIndex_;
    capacity_ = other.capacity_;
    size_ = other.size_;
    hash_ = other.hash_;
    equal_ = other.equal_;

    slots_ = new Slot<K, V>[capacity_];
    for (size_t i = 0; i < capacity_; ++i) {
      slots_[i] = other.slots_[i];
    }
  }
  return *this;
}

template < class K, class V, class H, class E >
muh::HashTable< K, V, H, E >& muh::HashTable< K, V, H, E >::operator=(HashTable&& other) noexcept
{
  if (this != &other) {
    delete[] slots_;

    bucketCapacity_ = other.bucketCapacity_;
    bucketCount_ = other.bucketCount_;
    otherflowBucketIndex_ = other.otherflowBucketIndex_;
    capacity_ = other.capacity_;
    size_ = other.size_;
    slots_ = other.slots_;
    hash_ = std::move(other.hash_);
    equal_ = std::move(other.equal_);

    other.slots_ = nullptr;
    other.bucketCapacity_ = 0;
    other.bucketCount_ = 0;
    other.otherflowBucketIndex_ = 0;
    other.capacity_ = 0;
    other.size_ = 0;
  }
  return *this;
}

template < class K, class V, class H, class E >
muh::HashTable< K, V, H, E >::~HashTable() noexcept
{
  delete [] slots_;
}

template < class K, class V, class H, class E >
size_t muh::HashTable< K, V, H, E >::getBucketStart(const K& k) const
{
  return (hash_(k) % bucketCount_) * bucketCapacity_;
}

template < class K, class V, class H, class E >
bool muh::HashTable< K, V, H, E >::findSlot(const K& k, size_t& id) const
{
  size_t startBucket = getBucketStart(k);
  size_t endBucket = startBucket + bucketCapacity_;
  for (size_t i = startBucket; i < endBucket; ++i)
  {
    if (slots_[i].state_ == State::EMPTY)
    {
      break;
    }
    if (slots_[i].state_ == State::OCCUPIED && equal_(slots_[i].key_, k))
    {
      id = i;
      return true;
    }
  }

  size_t startOtherflow = otherflowBucketIndex_;
  size_t endOtherflow = startOtherflow + bucketCapacity_;
  for (size_t i = startOtherflow; i < endOtherflow; ++i)
  {
    if (slots_[i].state_ == State::EMPTY)
    {
      break;
    }
    if (slots_[i].state_ == State::OCCUPIED && equal_(slots_[i].key_, k))
    {
      id = i;
      return true;
    }
  }
  return false;
}

template < class K, class V, class H, class E >
bool muh::HashTable< K, V, H, E >::findFreeSlot(const K& k, size_t& id) const
{
  size_t startBucket = getBucketStart(k);
  size_t endBucket = startBucket + bucketCapacity_;
  for (size_t i = startBucket; i < endBucket; ++i)
{
    if (slots_[i].state_ != State::OCCUPIED)
    {
      id = i;
      return true;
    }
  }

  size_t startOtherflow = otherflowBucketIndex_;
  size_t endOtherflow = startOtherflow + bucketCapacity_;
  for (size_t i = startOtherflow; i < endOtherflow; ++i)
  {
    if (slots_[i].state_ != State::OCCUPIED)
    {
      id = i;
      return true;
    }
  }
  return false;
}

template <class K, class V, class H, class E>
void muh::HashTable< K, V, H, E >::add(const K& k, const V& v)
{
  size_t slotIndex;
  bool result = findSlot(k, slotIndex);
  if (result)
  {
    slots_[slotIndex].value_ = v;
    return;
  }
  result = findFreeSlot(k, slotIndex);
  if (!result)
  {
    throw std::runtime_error("Hash-Table is otherflow: Not find free slot for data");
  }
  slots_[slotIndex] = Slot< K, V >(k, v);
  ++size_;
}

template <class K, class V, class H, class E>
V muh::HashTable< K, V, H, E >::drop(const K& k)
{
  size_t slotIndex;
  if (!findSlot(k, slotIndex))
  {
    throw std::out_of_range("Key not found");
  }
  V val = slots_[slotIndex].value_;
  slots_[slotIndex].state_ = State::DELETE;
  --size_;
  return val;
}

template <class K, class V, class H, class E>
bool muh::HashTable< K, V, H, E >::has(const K& k) const noexcept
{
  size_t dummy;
  return findSlot(k, dummy);
}

template <class K, class V, class H, class E>
void muh::HashTable< K, V, H, E >::rehash(size_t countBucket, size_t newBucketCapacity)
{
  HashTable< K, V, H, E > newTable(newBucketCapacity, countBucket);
  for (size_t i = 0; i < capacity_; ++i)
  {
    if (slots_[i].state_ == State::OCCUPIED)
    {
      newTable.add(slots_[i].key_, slots_[i].value_);
    }
  }

  *this = std::move(newTable);
}

template <class K, class V, class H, class E>
size_t muh::HashTable< K, V, H, E >::size() const noexcept
{
  return size_;
}

template <class K, class V, class H, class E>
size_t muh::HashTable< K, V, H, E >::bucketCapacity() const noexcept
{
  return bucketCapacity_;
}

template <class K, class V, class H, class E>
size_t muh::HashTable< K, V, H, E >::bucketCount() const noexcept
{
  return bucketCount_;
}

template <class K, class V, class H, class E>
size_t muh::HashTable< K, V, H, E >::capacity() const noexcept
{
  return capacity_;
}

template < class K, class V, class H, class E>
V& muh::HashTable< K, V, H, E >::get(const K& k)
{
  size_t id = 0;
  if (!findSlot(k, id))
  {
    throw std::out_of_range("Not find the data with this key");
  }
  return slots_[id].value_;
}

template < class K, class V, class H, class E>
const V& muh::HashTable< K, V, H, E >::cget(const K& k)
{
  size_t id = 0;
  if (!findSlot(k, id))
  {
    throw std::out_of_range("Not find the data with this key");
  }
  return slots_[id].value_;
}

template < class K, class V, class H, class E >
void muh::HashTable< K, V, H, E >::clear() noexcept
{
  for (size_t i = 0; i < capacity_; ++i)
  {
    slots_[i].state_ = State::EMPTY;
  }
  size_ = 0;
}

template < class K, class V, class H, class E >
muh::HTIter< K, V, H, E >::HTIter(HashTable< K, V, H, E >* ht, size_t index):
  ht_(ht),
  currentIndex_(index)
{
  if (ht_ && currentIndex_ < ht_->capacity_)
  {
    advanceToNextOccupied();
  }
}

template < class K, class V, class H, class E >
void muh::HTIter< K, V, H, E >::advanceToNextOccupied()
{
  if (!ht_)
  {
    return;
  }
  bool isTrueInd = currentIndex_ < ht_->capacity_;
  while (isTrueInd && ht_->slots_[currentIndex_].state_ != State::OCCUPIED)
  {
    ++currentIndex_;
    isTrueInd = currentIndex_ < ht_->capacity_;
  }
}

template < class K, class V, class H, class E >
muh::Slot< K, V >& muh::HTIter< K, V, H, E >::operator*() const
{
  if (!ht_ || currentIndex_ >= ht_->capacity_)
  {
    throw std::out_of_range("Iterator out of range");
  }
  return ht_->slots_[currentIndex_];
}

template < class K, class V, class H, class E >
muh::Slot< K, V >* muh::HTIter< K, V, H, E >::operator->()
{
  if (!ht_ || currentIndex_ >= ht_->capacity_)
  {
    throw std::out_of_range("Iterator out of range");
  }
  return std::addressof(ht_->slots_[currentIndex_]);
}


template < class K, class V, class H, class E >
bool muh::HTIter< K, V, H, E >::operator==(const HTIter& other) const
{
  return ht_ == other.ht_ && currentIndex_ == other.currentIndex_;
}

template < class K, class V, class H, class E >
bool muh::HTIter< K, V, H, E >::operator!=(const HTIter& other) const
{
  return !(*this == other);
}

template < class K, class V, class H, class E >
muh::HTIter< K, V, H, E >& muh::HTIter< K, V, H, E >::operator++()
{
  if (ht_ && currentIndex_ < ht_->capacity_)
  {
    ++currentIndex_;
    advanceToNextOccupied();
  }
  return *this;
}

template < class K, class V, class H, class E >
muh::HTIter< K, V, H, E > muh::HTIter< K, V, H, E >::operator++(int)
{
  HTIter temp = *this;
  ++(*this);
  return temp;
}

template < class K, class V, class H, class E >
muh::HTIter< K, V, H, E > muh::HashTable< K, V, H, E >::begin()
{
  return HTIter< K, V, H, E >(this, 0);
}

template < class K, class V, class H, class E >
muh::HTIter< K, V, H, E > muh::HashTable< K, V, H, E >::end()
{
  return HTIter< K, V, H, E >(this, capacity_);
}

template < class K, class V, class H, class E >
muh::HTCIter< K, V, H, E >::HTCIter(const HashTable< K, V, H, E >* ht, size_t index):
  ht_(ht),
  currentIndex_(index)
{
  if (ht_ && currentIndex_ < ht_->capacity_)
  {
    advanceToNextOccupied();
  }
}

template < class K, class V, class H, class E >
void muh::HTCIter< K, V, H, E >::advanceToNextOccupied()
{
  if (!ht_)
  {
    return;
  }
  bool isTrueInd = currentIndex_ < ht_->capacity_;
  while (isTrueInd && ht_->slots_[currentIndex_].state_ != State::OCCUPIED)
  {
    ++currentIndex_;
    isTrueInd = currentIndex_ < ht_->capacity_;
  }
}

template < class K, class V, class H, class E >
const muh::Slot< K, V >& muh::HTCIter< K, V, H, E >::operator*() const
{
  if (!ht_ || currentIndex_ >= ht_->capacity_)
  {
    throw std::out_of_range("Iterator out of range");
  }
  return ht_->slots_[currentIndex_];
}

template < class K, class V, class H, class E >
const muh::Slot< K, V >* muh::HTCIter< K, V, H, E >::operator->() const
{
  if (!ht_ || currentIndex_ >= ht_->capacity_)
  {
    throw std::out_of_range("Iterator out of range");
  }
  return std::addressof(ht_->slots_[currentIndex_]);
}


template < class K, class V, class H, class E >
bool muh::HTCIter< K, V, H, E >::operator==(const HTCIter& other) const
{
  return ht_ == other.ht_ && currentIndex_ == other.currentIndex_;
}

template < class K, class V, class H, class E >
bool muh::HTCIter< K, V, H, E >::operator!=(const HTCIter& other) const
{
  return !(*this == other);
}

template < class K, class V, class H, class E >
muh::HTCIter< K, V, H, E >& muh::HTCIter< K, V, H, E >::operator++()
{
  if (ht_ && currentIndex_ < ht_->capacity_)
  {
    ++currentIndex_;
    advanceToNextOccupied();
  }
  return *this;
}

template < class K, class V, class H, class E >
muh::HTCIter< K, V, H, E > muh::HTCIter< K, V, H, E >::operator++(int)
{
  HTCIter temp = *this;
  ++(*this);
  return temp;
}

template < class K, class V, class H, class E >
muh::HTCIter< K, V, H, E > muh::HashTable< K, V, H, E >::cbegin() const
{
  return HTCIter< K, V, H, E >(this, 0);
}

template < class K, class V, class H, class E >
muh::HTCIter< K, V, H, E > muh::HashTable< K, V, H, E >::cend() const
{
  return HTCIter< K, V, H, E >(this, capacity_);
}
#endif
