#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>
#include "vector.hpp"

namespace alisov
{
  template < class Key, class Value, class Hash, class Equal >
  class HashTable
  {
  private:
    struct Node
    {
      Key key;
      Value value;
      Node():
        key(Key()),
        value(Value())
      {}
      Node(Key k, Value v):
        key(k),
        value(v)
      {}
    };

    Vector< Vector< Node > > buckets_;
    size_t element_count_;
    size_t slots_count_;
    Hash hasher_;
    Equal equal_;

    size_t getBucketIndex(const Key &k) const
    {
      return slots_count_ == 0 ? 0 : hasher_(k) % slots_count_;
    }

  public:
    HashTable():
      element_count_(0),
      slots_count_(0)
    {}

    explicit HashTable(size_t slots):
      element_count_(0),
      slots_count_(slots)
    {
      buckets_.reserve(slots);
      for (size_t i = 0; i < slots; ++i) {
        buckets_.push_back(Vector< Node >());
      }
    }

    ~HashTable() = default;

    size_t size() const
    {
      return element_count_;
    }

    size_t slotsCount() const
    {
      return slots_count_;
    }

    bool empty() const
    {
      return element_count_ == 0;
    }
    bool insert(const Key &k, const Value &v)
    {
      if (slots_count_ == 0) {
        slots_count_ = 16;
        buckets_.reserve(16);
        for (size_t i = 0; i < 16; ++i) {
          buckets_.push_back(Vector< Node >());
        }
      }
      size_t idx = getBucketIndex(k);
      for (size_t i = 0; i < buckets_[idx].size(); ++i) {
        if (equal_(buckets_[idx][i].key, k)) {
          return false;
        }
      }
      buckets_[idx].push_back(Node(k, v));
      ++element_count_;
      return true;
    }

    void add(Key k, Value v)
    {
      if (!insert(k, v)) {
        throw std::runtime_error("Key duplicate");
      }
    }

    bool contains(const Key &k) const
    {
      if (slots_count_ == 0) {
        return false;
      }
      size_t idx = getBucketIndex(k);
      for (size_t i = 0; i < buckets_[idx].size(); ++i) {
        if (equal_(buckets_[idx][i].key, k)) {
          return true;
        }
      }
      return false;
    }

    bool has(Key k)
    {
      return contains(k);
    }
    Value &at(const Key &k)
    {
      if (slots_count_ == 0) {
        throw std::out_of_range("Not found");
      }
      size_t idx = getBucketIndex(k);
      for (size_t i = 0; i < buckets_[idx].size(); ++i) {
        if (equal_(buckets_[idx][i].key, k)) {
          return buckets_[idx][i].value;
        }
      }
      throw std::out_of_range("Not found");
    }

    const Value &at(const Key &k) const
    {
      if (slots_count_ == 0) {
        throw std::out_of_range("Not found");
      }
      size_t idx = getBucketIndex(k);
      for (size_t i = 0; i < buckets_[idx].size(); ++i) {
        if (equal_(buckets_[idx][i].key, k)) {
          return buckets_[idx][i].value;
        }
      }
      throw std::out_of_range("Not found");
    }

    Value &operator[](const Key &k)
    {
      if (slots_count_ == 0) {
        slots_count_ = 16;
        buckets_.reserve(16);
        for (size_t i = 0; i < 16; ++i) {
          buckets_.push_back(Vector< Node >());
        }
      }
      size_t idx = getBucketIndex(k);
      for (size_t i = 0; i < buckets_[idx].size(); ++i) {
        if (equal_(buckets_[idx][i].key, k)) {
          return buckets_[idx][i].value;
        }
      }
      buckets_[idx].push_back(Node(k, Value()));
      ++element_count_;
      return buckets_[idx][buckets_[idx].size() - 1].value;
    }
    bool erase(const Key &k)
    {
      if (slots_count_ == 0) {
        return false;
      }
      size_t idx = getBucketIndex(k);
      for (size_t i = 0; i < buckets_[idx].size(); ++i) {
        if (equal_(buckets_[idx][i].key, k)) {
          buckets_[idx].erase(i);
          --element_count_;
          return true;
        }
      }
      return false;
    }

    Value drop(Key k)
    {
      Value val = at(k);
      erase(k);
      return val;
    }

    void rehash(size_t slots)
    {
      HashTable< Key, Value, Hash, Equal > new_table(slots);
      for (size_t i = 0; i < slots_count_; ++i) {
        for (size_t j = 0; j < buckets_[i].size(); ++j) {
          new_table.insert(buckets_[i][j].key, buckets_[i][j].value);
        }
      }
      buckets_ = std::move(new_table.buckets_);
      element_count_ = new_table.element_count_;
      slots_count_ = new_table.slots_count_;
    }

    Vector< Key > getAllKeys() const
    {
      Vector< Key > keys;
      for (size_t i = 0; i < slots_count_; ++i) {
        for (size_t j = 0; j < buckets_[i].size(); ++j) {
          keys.push_back(buckets_[i][j].key);
        }
      }
      return keys;
    }
    void swap(HashTable &other) noexcept
    {
      buckets_.swap(other.buckets_);
      std::swap(element_count_, other.element_count_);
      std::swap(slots_count_, other.slots_count_);
      std::swap(hasher_, other.hasher_);
      std::swap(equal_, other.equal_);
    }
  };
}

#endif
