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
      while (index_ < capacity_ && current_ == nullptr) {
        ++index_;
        if (index_ < capacity_) {
          current_ = table_[index_];
        }
      }
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
      while (index_ < capacity_ && current_ == nullptr) {
        ++index_;
        if (index_ < capacity_) {
          current_ = table_[index_];
        }
      }
    }
  };
  
}
#endif
