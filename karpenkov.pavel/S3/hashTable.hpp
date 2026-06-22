#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <utility>
#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP
enum State { EMPTY, OCCUPIED, TOMBSTONE };
template <class Key, class Value, class Hash, class Equal> class Iterator;
template <class Key, class Value, class Hash, class Equal> class HashTable {
  friend class Iterator<Key, Value, Hash, Equal>;

public:
  HashTable(Hash hash, Equal equal)
      : size_(0), capacity_(10), hash_(hash), equal_(equal) {
    table_ = new Unit[capacity_];
  }
  HashTable(const HashTable &other)
      : size_(other.size_), capacity_(other.capacity_), hash_(other.hash_),
        equal_(other.equal_) {
    table_ = new Unit[capacity_];
    for (size_t i = 0; i < capacity_; ++i) {
      table_[i] = other.table_[i];
    }
  }
  HashTable &operator=(const HashTable &other) {
    if (this == &other) {
      return *this;
    }
    Unit *new_table = new Unit[other.capacity_];
    for (size_t i = 0; i < other.capacity_; ++i) {
      new_table[i] = other.table_[i];
    }
    delete[] table_;
    table_ = new_table;
    size_ = other.size_;
    capacity_ = other.capacity_;
    hash_ = other.hash_;
    equal_ = other.equal_;
    return *this;
  }
  HashTable(HashTable &&other) noexcept
      : table_(other.table_), size_(other.size_), capacity_(other.capacity_),
        hash_(std::move(other.hash_)), equal_(std::move(other.equal_)) {
    other.table_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
  }
  HashTable &operator=(HashTable &&other) noexcept {
    if (this == &other) {
      return *this;
    }
    delete[] table_;
    table_ = other.table_;
    size_ = other.size_;
    capacity_ = other.capacity_;
    hash_ = std::move(other.hash_);
    equal_ = std::move(other.equal_);
    other.table_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
    return *this;
  }
  ~HashTable() { delete[] table_; }
  struct Unit {
    Key key;
    Value value;
    State state = EMPTY;
  };
  size_t size() const { return size_; }
  Iterator<Key, Value, Hash, Equal> begin() {
    return Iterator<Key, Value, Hash, Equal>(table_, table_ + capacity_);
  }
  Iterator<Key, Value, Hash, Equal> end() {
    return Iterator<Key, Value, Hash, Equal>(table_ + capacity_,
                                             table_ + capacity_);
  }
  void add(Key k, Value v) {
    size_t index = hash_(k) % capacity_;
    for (size_t i = 0; i < capacity_; ++i) {
      size_t cur = (index + i) % capacity_;
      if (table_[cur].state == EMPTY || table_[cur].state == TOMBSTONE) {
        table_[cur].key = k;
        table_[cur].value = v;
        table_[cur].state = OCCUPIED;
        size_++;
        return;
      }
      if (table_[cur].state == OCCUPIED && equal_(table_[cur].key, k)) {
        table_[cur].value = v;
        return;
      }
    }
    throw std::runtime_error("HashTable is full");
  }
  bool has(Key k) {
    size_t index = hash_(k) % capacity_;
    for (size_t i = 0; i < capacity_; ++i) {
      size_t cur = (index + i) % capacity_;
      if (table_[cur].state == EMPTY) {
        return false;
      } else if (table_[cur].state == OCCUPIED && equal_(table_[cur].key, k)) {
        return true;
      }
    }
    return false;
  }
  Value &at(Key k) {
    size_t index = hash_(k) % capacity_;
    for (size_t i = 0; i < capacity_; ++i) {
      size_t cur = (index + i) % capacity_;
      if (table_[cur].state == EMPTY) {
        throw std::runtime_error("no element with such key");
      }
      if (equal_(table_[cur].key, k)) {
        return table_[cur].value;
      }
    }
    throw std::runtime_error("no element with such key");
  }
  Value drop(Key k) {
    size_t index = hash_(k) % capacity_;
    for (size_t i = 0; i < capacity_; ++i) {
      size_t cur = (index + i) % capacity_;
      if (table_[cur].state == OCCUPIED && equal_(table_[cur].key, k)) {
        table_[cur].state = TOMBSTONE;
        --size_;
        return table_[cur].value;
      }
      if (table_[cur].state == EMPTY) {
        throw std::runtime_error("key not found");
      }
    }
    throw std::runtime_error("key not found");
  }
  void rehash(size_t slots) {
    Unit *old_table = table_;
    size_t old_capacity = capacity_;

    capacity_ = slots;
    table_ = new Unit[capacity_];
    size_ = 0;

    for (size_t i = 0; i < old_capacity; ++i) {
      if (old_table[i].state == OCCUPIED) {
        add(old_table[i].key, old_table[i].value);
      }
    }

    delete[] old_table;
  }

private:
  Unit *table_;
  size_t size_;
  size_t capacity_;
  Hash hash_;
  Equal equal_;
};
template <class Key, class Value, class Hash, class Equal> class Iterator {
  using Unit = typename HashTable<Key, Value, Hash, Equal>::Unit;

public:
  Iterator(Unit *start, Unit *end) : current_(start), end_(end) {
    skip_invalid();
  }
  Iterator &operator++() {
    ++current_;
    skip_invalid();
    return *this;
  }
  bool operator!=(const Iterator &other) const {
    return current_ != other.current_;
  }
  bool operator==(const Iterator &other) const {
    return current_ == other.current_;
  }
  Unit &operator*() { return *current_; }

private:
  void skip_invalid() {
    while (current_ != end_ && current_->state != OCCUPIED) {
      current_++;
    }
  }
  Unit *current_;
  Unit *end_;
};

#endif
