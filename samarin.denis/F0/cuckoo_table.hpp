#ifndef CUCKOO_TABLE_HPP
#define CUCKOO_TABLE_HPP

#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>

namespace samarin {

  namespace detail {
    template< class Key, class Value >
    struct entry_t {
      std::pair< Key, Value > data;
      bool occupied = false;
    };

    std::size_t cuckooMix(std::size_t x);
  }

  template< class Key, class Value, class Hash, class Equal >
  class CuckooTable;

  template< class Key, class Value >
  class CuckooConstIterator {
  public:
    const std::pair< Key, Value >& operator*() const
    {
      return slots_[index_].data;
    }

    const std::pair< Key, Value >* operator->() const
    {
      return std::addressof(slots_[index_].data);
    }

    CuckooConstIterator< Key, Value >& operator++()
    {
      ++index_;
      skip();
      return *this;
    }

    CuckooConstIterator< Key, Value > operator++(int)
    {
      CuckooConstIterator< Key, Value > tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator==(const CuckooConstIterator< Key, Value >& other) const
    {
      return index_ == other.index_;
    }

    bool operator!=(const CuckooConstIterator< Key, Value >& other) const
    {
      return index_ != other.index_;
    }

  private:
    template< class K, class V, class H, class E >
    friend class CuckooTable;

    const detail::entry_t< Key, Value >* slots_;
    std::size_t capacity_;
    std::size_t index_;

    CuckooConstIterator(const detail::entry_t< Key, Value >* slots, std::size_t capacity, std::size_t index):
      slots_(slots),
      capacity_(capacity),
      index_(index)
    {
      skip();
    }

    void skip()
    {
      while (index_ < capacity_ && !slots_[index_].occupied) {
        ++index_;
      }
    }
  };

  template< class Key, class Value, class Hash = std::hash< Key >, class Equal = std::equal_to< Key > >
  class CuckooTable {
  public:
    using const_iterator = CuckooConstIterator< Key, Value >;

    CuckooTable():
      slots_(new detail::entry_t< Key, Value >[default_capacity]),
      capacity_(default_capacity),
      size_(0)
    {}

    explicit CuckooTable(std::size_t slots):
      slots_(new detail::entry_t< Key, Value >[atLeastMin(slots)]),
      capacity_(atLeastMin(slots)),
      size_(0)
    {}

    CuckooTable(const CuckooTable< Key, Value, Hash, Equal >& other):
      slots_(new detail::entry_t< Key, Value >[other.capacity_]),
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

    CuckooTable(CuckooTable< Key, Value, Hash, Equal >&& other) noexcept:
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

    ~CuckooTable()
    {
      delete[] slots_;
    }

    CuckooTable< Key, Value, Hash, Equal >& operator=(const CuckooTable< Key, Value, Hash, Equal >& other)
    {
      if (this != std::addressof(other)) {
        CuckooTable< Key, Value, Hash, Equal > tmp(other);
        swap(tmp);
      }
      return *this;
    }

    CuckooTable< Key, Value, Hash, Equal >& operator=(CuckooTable< Key, Value, Hash, Equal >&& other) noexcept
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
      std::size_t found = locate(key);
      if (found == capacity_) {
        insert(key, Value());
        found = locate(key);
      }
      return slots_[found].data.second;
    }

    void add(const Key& key, const Value& value)
    {
      insert(key, value);
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
      slots_[slot].occupied = false;
      --size_;
      return value;
    }

    void rehash(std::size_t slots)
    {
      std::size_t target = atLeastMin(slots);
      if (target < size_ * 2) {
        target = size_ * 2;
      }
      CuckooTable< Key, Value, Hash, Equal > rebuilt(target);
      for (std::size_t i = 0; i < capacity_; ++i) {
        if (slots_[i].occupied) {
          rebuilt.insert(slots_[i].data.first, slots_[i].data.second);
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
    static constexpr std::size_t min_capacity = 2;
    static constexpr std::size_t default_capacity = 16;
    static constexpr std::size_t load_num = 1;
    static constexpr std::size_t load_den = 2;
    static constexpr std::size_t growth = 2;

    static std::size_t atLeastMin(std::size_t slots)
    {
      return slots < min_capacity ? min_capacity : slots;
    }

    detail::entry_t< Key, Value >* slots_;
    std::size_t capacity_;
    std::size_t size_;
    Hash hash_;
    Equal equal_;

    void swap(CuckooTable< Key, Value, Hash, Equal >& other) noexcept
    {
      std::swap(slots_, other.slots_);
      std::swap(capacity_, other.capacity_);
      std::swap(size_, other.size_);
      std::swap(hash_, other.hash_);
      std::swap(equal_, other.equal_);
    }

    std::size_t indexOf(std::size_t hashValue) const
    {
      return hashValue % capacity_;
    }

    std::size_t home1(const Key& key) const
    {
      return indexOf(hash_(key));
    }

    std::size_t home2(const Key& key) const
    {
      return indexOf(detail::cuckooMix(hash_(key)));
    }

    bool wouldOverload(std::size_t extra) const
    {
      return (size_ + extra) * load_den > capacity_ * load_num;
    }

    std::size_t alternateHome(const Key& key, std::size_t current) const
    {
      const std::size_t first = home1(key);
      return (current == first) ? home2(key) : first;
    }

    std::size_t maxKicks() const
    {
      std::size_t bound = 1;
      for (std::size_t cap = capacity_; cap > 1; cap >>= 1) {
        ++bound;
      }
      return bound * 4 + 4;
    }

    bool slotFree(std::size_t index) const
    {
      return !slots_[index].occupied;
    }

    bool slotMatches(std::size_t index, const Key& key) const
    {
      return slots_[index].occupied && equal_(slots_[index].data.first, key);
    }

    std::size_t locate(const Key& key) const
    {
      const std::size_t base = hash_(key);
      const std::size_t first = indexOf(base);
      if (slotMatches(first, key)) {
        return first;
      }
      const std::size_t second = indexOf(detail::cuckooMix(base));
      if (slotMatches(second, key)) {
        return second;
      }
      return capacity_;
    }

    bool place(std::pair< Key, Value >& cur)
    {
      std::size_t pos = home1(cur.first);
      const std::size_t limit = maxKicks();
      for (std::size_t kick = 0; kick < limit; ++kick) {
        if (slotFree(pos)) {
          slots_[pos].data = cur;
          slots_[pos].occupied = true;
          return true;
        }
        std::swap(cur, slots_[pos].data);
        pos = alternateHome(cur.first, pos);
      }
      return false;
    }

    void insert(const Key& key, const Value& value)
    {
      const std::size_t found = locate(key);
      if (found != capacity_) {
        slots_[found].data.second = value;
        return;
      }
      if (wouldOverload(1)) {
        rehash(capacity_ * growth);
      }
      std::pair< Key, Value > cur(key, value);
      while (!place(cur)) {
        rehash(capacity_ * growth);
      }
      ++size_;
    }
  };

}

#endif
