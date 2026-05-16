#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <stdexcept>
#include <utility>
#include <functional>
#include <cstddef>
#include <algorithm>

namespace petrov {

  template< class Key, class Value >
  struct Node {
    Key key;
    Value value;
    Node* next;

    Node(const Key& k, const Value& v):
      key(k),
      value(v),
      next(nullptr)
    {}
  };

  template< class Key, class Value, class Hash, class Equal >
  class HTIterator {
    template< class K, class V, class H, class E >
    friend class HashTable;

  public:
    using value_type = std::pair< Key, Value >;
    using reference = value_type&;
    using pointer = value_type*;

    HTIterator():
      table_(nullptr),
      current_(nullptr),
      index_(0),
      capacity_(0)
    {}

    HTIterator& operator++()
    {
      if (current_) {
        current_ = current_->next;
        if (!current_) {
          ++index_;
          findNext();
        }
      }
      return *this;
    }

    bool operator==(const HTIterator& other) const
    {
      return current_ == other.current_;
    }

    bool operator!=(const HTIterator& other) const
    {
      return !(*this == other);
    }

    value_type operator*() const
    {
      return {current_->key, current_->value};
    }

  private:
    const Node< Key, Value >** table_;
    const Node< Key, Value >* current_;
    size_t index_;
    size_t capacity_;

    HTIterator(const Node< Key, Value >** tbl, const Node< Key, Value >* curr, size_t idx, size_t cap):
      table_(tbl),
      current_(curr),
      index_(idx),
      capacity_(cap)
    {}

    void findNext()
    {
      while (index_ < capacity_) {
        if (table_[index_]) {
          current_ = table_[index_];
          return;
        }
        ++index_;
      }
      current_ = nullptr;
    }
  };

  template< class Key, class Value, class Hash, class Equal >
  class HTConstIterator {
    template< class K, class V, class H, class E >
    friend class HashTable;

  public:
    using value_type = std::pair< const Key, Value >;
    using reference = const value_type&;
    using pointer = const value_type*;

    HTConstIterator():
      table_(nullptr),
      current_(nullptr),
      index_(0),
      capacity_(0)
    {}

    HTConstIterator& operator++()
    {
      if (current_) {
        current_ = current_->next;
        if (!current_) {
          ++index_;
          findNext();
        }
      }
      return *this;
    }

    bool operator==(const HTConstIterator& other) const
    {
      return current_ == other.current_;
    }

    bool operator!=(const HTConstIterator& other) const
    {
      return !(*this == other);
    }

    value_type operator*() const
    {
      return {current_->key, current_->value};
    }

  private:
    const Node< Key, Value >* const* table_;
    const Node< Key, Value >* current_;
    size_t index_;
    size_t capacity_;

    HTConstIterator(const Node< Key, Value >* const* tbl, const Node< Key, Value >* curr, size_t idx, size_t cap):
      table_(tbl),
      current_(curr),
      index_(idx),
      capacity_(cap)
    {}

    void findNext()
    {
      while (index_ < capacity_) {
        if (table_[index_]) {
          current_ = table_[index_];
          return;
        }
        ++index_;
      }
      current_ = nullptr;
    }
  };

  template< class Key, class Value, class Hash, class Equal >
  class HashTable {
  public:
    using iterator = HTIterator< Key, Value, Hash, Equal >;
    using const_iterator = HTConstIterator< Key, Value, Hash, Equal >;

    explicit HashTable(size_t slots):
      table_(new Node< Key, Value >*[slots]()),
      capacity_(slots),
      size_(0),
      hash_(),
      equal_()
    {}

    HashTable(const HashTable& other):
      table_(new Node< Key, Value >*[other.capacity_]()),
      capacity_(other.capacity_),
      size_(0),
      hash_(other.hash_),
      equal_(other.equal_)
    {
      try {
        for (size_t i = 0; i < other.capacity_; ++i) {
          Node< Key, Value >* curr = other.table_[i];
          while (curr) {
            add(curr->key, curr->value);
            curr = curr->next;
          }
        }
      } catch (...) {
        clear();
        delete[] table_;
        throw;
      }
    }

    HashTable(HashTable&& other) noexcept:
      table_(other.table_),
      capacity_(other.capacity_),
      size_(other.size_),
      hash_(std::move(other.hash_)),
      equal_(std::move(other.equal_))
    {
      other.table_ = nullptr;
      other.capacity_ = 0;
      other.size_ = 0;
    }

    ~HashTable()
    {
      clear();
      delete[] table_;
    }

    HashTable& operator=(HashTable other)
    {
      swap(other);
      return *this;
    }

    void swap(HashTable& other) noexcept
    {
      std::swap(table_, other.table_);
      std::swap(capacity_, other.capacity_);
      std::swap(size_, other.size_);
      std::swap(hash_, other.hash_);
      std::swap(equal_, other.equal_);
    }

    void add(const Key& k, const Value& v)
    {
      size_t bucket = getBucket(k);
      Node< Key, Value >* newNode = new Node< Key, Value >(k, v);
      newNode->next = table_[bucket];
      table_[bucket] = newNode;
      ++size_;
    }

    Value& get(const Key& k)
    {
      size_t bucket = getBucket(k);
      Node< Key, Value >* curr = table_[bucket];
      while (curr) {
        if (equal_(curr->key, k)) {
          return curr->value;
        }
        curr = curr->next;
      }
      throw std::out_of_range("Key not found");
    }

    const Value& get(const Key& k) const
    {
      size_t bucket = getBucket(k);
      Node< Key, Value >* curr = table_[bucket];
      while (curr) {
        if (equal_(curr->key, k)) {
          return curr->value;
        }
        curr = curr->next;
      }
      throw std::out_of_range("Key not found");
    }

    bool has(const Key& k) const
    {
      size_t bucket = getBucket(k);
      Node< Key, Value >* curr = table_[bucket];
      while (curr) {
        if (equal_(curr->key, k)) {
          return true;
        }
        curr = curr->next;
      }
      return false;
    }

    void drop(const Key& k)
    {
      size_t bucket = getBucket(k);
      Node< Key, Value >* curr = table_[bucket];
      Node< Key, Value >* prev = nullptr;

      while (curr) {
        if (equal_(curr->key, k)) {
          if (prev) {
            prev->next = curr->next;
          } else {
            table_[bucket] = curr->next;
          }
          delete curr;
          --size_;
          return;
        }
        prev = curr;
        curr = curr->next;
      }
      throw std::out_of_range("Key not found");
    }

    void rehash(size_t slots)
    {
      HashTable temp(slots);
      temp.hash_ = hash_;
      temp.equal_ = equal_;

      for (size_t i = 0; i < capacity_; ++i) {
        Node< Key, Value >* curr = table_[i];
        while (curr) {
          temp.add(curr->key, curr->value);
          curr = curr->next;
        }
      }

      swap(temp);
    }

    iterator begin()
    {
      for (size_t i = 0; i < capacity_; ++i) {
        if (table_[i]) {
          return iterator(const_cast< const Node< Key, Value >** >(table_), table_[i], i, capacity_);
        }
      }
      return end();
    }

    iterator end()
    {
      return iterator(const_cast< const Node< Key, Value >** >(table_), nullptr, capacity_, capacity_);
    }

    const_iterator begin() const
    {
      return cbegin();
    }

    const_iterator end() const
    {
      return cend();
    }

    const_iterator cbegin() const
    {
      for (size_t i = 0; i < capacity_; ++i) {
        if (table_[i]) {
          return const_iterator(table_, table_[i], i, capacity_);
        }
      }
      return cend();
    }

    const_iterator cend() const
    {
      return const_iterator(table_, nullptr, capacity_, capacity_);
    }

    size_t size() const
    {
      return size_;
    }

    bool empty() const
    {
      return size_ == 0;
    }

  private:
    Node< Key, Value >** table_;
    size_t capacity_;
    size_t size_;
    Hash hash_;
    Equal equal_;

    size_t getBucket(const Key& k) const
    {
      return hash_(k) % capacity_;
    }

    void clear()
    {
      for (size_t i = 0; i < capacity_; ++i) {
        Node< Key, Value >* curr = table_[i];
        while (curr) {
          Node< Key, Value >* temp = curr;
          curr = curr->next;
          delete temp;
        }
        table_[i] = nullptr;
      }
      size_ = 0;
    }
  };

  template< class Key, class Value, class Hash, class Equal >
  void swap(HashTable< Key, Value, Hash, Equal >& lhs, HashTable< Key, Value, Hash, Equal >& rhs) noexcept
  {
    lhs.swap(rhs);
  }
}

#endif

