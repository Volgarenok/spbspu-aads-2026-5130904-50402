#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>

namespace samarin {

  namespace detail {
    enum class SlotState {
      empty,
      occupied,
      deleted
    };

    template< class Key, class Value >
    struct Entry {
      std::pair< Key, Value > data;
      SlotState state = SlotState::empty;
    };
  }

  template< class Key, class Value, class Hash, class Equal >
  class HashTable;

  template< class Key, class Value >
  class HashConstIterator {
  public:
    const std::pair< Key, Value >& operator*() const
    {
      return slots_[index_].data;
    }

    const std::pair< Key, Value >* operator->() const
    {
      return std::addressof(slots_[index_].data);
    }

    HashConstIterator< Key, Value >& operator++()
    {
      ++index_;
      skip();
      return *this;
    }

    HashConstIterator< Key, Value > operator++(int)
    {
      HashConstIterator< Key, Value > tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator==(const HashConstIterator< Key, Value >& other) const
    {
      return index_ == other.index_;
    }

    bool operator!=(const HashConstIterator< Key, Value >& other) const
    {
      return index_ != other.index_;
    }

  private:
    template< class K, class V, class H, class E >
    friend class HashTable;

    const detail::Entry< Key, Value >* slots_;
    std::size_t capacity_;
    std::size_t index_;

    HashConstIterator(const detail::Entry< Key, Value >* slots, std::size_t capacity, std::size_t index):
      slots_(slots),
      capacity_(capacity),
      index_(index)
    {
      skip();
    }

    void skip()
    {
      while (index_ < capacity_ && slots_[index_].state != detail::SlotState::occupied) {
        ++index_;
      }
    }
  };

  template< class Key, class Value, class Hash = std::hash< Key >, class Equal = std::equal_to< Key > >
  class HashTable {
  public:
    using const_iterator = HashConstIterator< Key, Value >;

    HashTable():
      slots_(new detail::Entry< Key, Value >[default_capacity]),
      capacity_(default_capacity),
      size_(0)
    {}

    explicit HashTable(std::size_t slots):
      slots_(new detail::Entry< Key, Value >[atLeastMin(slots)]),
      capacity_(atLeastMin(slots)),
      size_(0)
    {}

    HashTable(const HashTable< Key, Value, Hash, Equal >& other):
      slots_(new detail::Entry< Key, Value >[other.capacity_]),
      capacity_(other.capacity_),
      size_(other.size_),
      hash_(other.hash_),
      equal_(other.equal_)
    {
      try {
        for (std::size_t i = 0; i < capacity_; ++i) {
          slots_[i] = other.slots_[i];
        }
      } catch (...) {
        delete[] slots_;
        throw;
      }
    }

    HashTable(HashTable< Key, Value, Hash, Equal >&& other) noexcept:
      slots_(other.slots_),
      capacity_(other.capacity_),
      size_(other.size_),
      hash_(other.hash_),
      equal_(other.equal_)
    {
      other.slots_ = nullptr;
      other.capacity_ = 0;
      other.size_ = 0;
    }

    ~HashTable()
    {
      delete[] slots_;
    }

    HashTable< Key, Value, Hash, Equal >& operator=(const HashTable< Key, Value, Hash, Equal >& other)
    {
      if (this != std::addressof(other)) {
        HashTable< Key, Value, Hash, Equal > tmp(other);
        swap(tmp);
      }
      return *this;
    }

    HashTable< Key, Value, Hash, Equal >& operator=(HashTable< Key, Value, Hash, Equal >&& other) noexcept
    {
      if (this != std::addressof(other)) {
        delete[] slots_;
        slots_ = other.slots_;
        capacity_ = other.capacity_;
        size_ = other.size_;
        other.slots_ = nullptr;
        other.capacity_ = 0;
        other.size_ = 0;
      }
      return *this;
    }

    Value& operator[](const Key& key)
    {
      const std::size_t slot = findOrReserve(key);
      return slots_[slot].data.second;
    }

    void add(const Key& key, const Value& value)
    {
      const std::size_t slot = findOrReserve(key);
      slots_[slot].data.second = value;
    }

    Value& at(const Key& key)
    {
      const std::size_t slot = locate(key);
      if (slot == capacity_) {
        throw std::out_of_range("key is missing");
      }
      return slots_[slot].data.second;
    }

    const Value& at(const Key& key) const
    {
      const std::size_t slot = locate(key);
      if (slot == capacity_) {
        throw std::out_of_range("key is missing");
      }
      return slots_[slot].data.second;
    }

    bool has(const Key& key) const
    {
      return locate(key) != capacity_;
    }

    const_iterator find(const Key& key) const
    {
      return const_iterator(slots_, capacity_, locate(key));
    }

    Value drop(const Key& key)
    {
      const std::size_t slot = locate(key);
      if (slot == capacity_) {
        throw std::out_of_range("key is missing");
      }
      Value value = std::move(slots_[slot].data.second);
      slots_[slot].state = detail::SlotState::deleted;
      --size_;
      return value;
    }

    void rehash(std::size_t slots)
    {
      if (slots < size_) {
        throw std::length_error("too few slots for rehash");
      }
      HashTable< Key, Value, Hash, Equal > rebuilt(slots);
      for (std::size_t i = 0; i < capacity_; ++i) {
        if (slots_[i].state == detail::SlotState::occupied) {
          rebuilt.add(slots_[i].data.first, slots_[i].data.second);
        }
      }
      swap(rebuilt);
    }

    std::size_t size() const
    {
      return size_;
    }

    std::size_t capacity() const
    {
      return capacity_;
    }

    bool empty() const
    {
      return size_ == 0;
    }

    const_iterator cbegin() const
    {
      return const_iterator(slots_, capacity_, 0);
    }

    const_iterator cend() const
    {
      return const_iterator(slots_, capacity_, capacity_);
    }

    const_iterator begin() const
    {
      return cbegin();
    }

    const_iterator end() const
    {
      return cend();
    }

  private:
    static constexpr std::size_t min_capacity = 1;
    static constexpr std::size_t default_capacity = 16;

    static std::size_t atLeastMin(std::size_t slots)
    {
      return slots < min_capacity ? min_capacity : slots;
    }

    detail::Entry< Key, Value >* slots_;
    std::size_t capacity_;
    std::size_t size_;
    Hash hash_;
    Equal equal_;

    void swap(HashTable< Key, Value, Hash, Equal >& other) noexcept
    {
      std::swap(slots_, other.slots_);
      std::swap(capacity_, other.capacity_);
      std::swap(size_, other.size_);
      std::swap(hash_, other.hash_);
      std::swap(equal_, other.equal_);
    }

    std::size_t startIndex(const Key& key) const
    {
      return hash_(key) % capacity_;
    }

    std::size_t locate(const Key& key) const
    {
      const std::size_t start = startIndex(key);
      for (std::size_t i = 0; i < capacity_; ++i) {
        const std::size_t idx = (start + i) % capacity_;
        if (slots_[idx].state == detail::SlotState::empty) {
          return capacity_;
        }
        if (slots_[idx].state == detail::SlotState::occupied && equal_(slots_[idx].data.first, key)) {
          return idx;
        }
      }
      return capacity_;
    }

    std::size_t findOrReserve(const Key& key)
    {
      const std::size_t start = startIndex(key);
      std::size_t freeSlot = capacity_;
      for (std::size_t i = 0; i < capacity_; ++i) {
        const std::size_t idx = (start + i) % capacity_;
        const detail::SlotState state = slots_[idx].state;
        if (state == detail::SlotState::occupied) {
          if (equal_(slots_[idx].data.first, key)) {
            return idx;
          }
        } else {
          if (freeSlot == capacity_) {
            freeSlot = idx;
          }
          if (state == detail::SlotState::empty) {
            break;
          }
        }
      }
      if (freeSlot == capacity_) {
        throw std::length_error("hash table is full");
      }
      slots_[freeSlot].data.first = key;
      slots_[freeSlot].data.second = Value();
      slots_[freeSlot].state = detail::SlotState::occupied;
      ++size_;
      return freeSlot;
    }
  };

}

#endif
