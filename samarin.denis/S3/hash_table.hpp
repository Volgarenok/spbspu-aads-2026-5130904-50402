#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <cstddef>
#include <functional>
#include <memory>
#include <stdexcept>
#include <utility>

namespace samarin {

  namespace detail {
    template< class Key, class Value >
    struct BucketNode {
      std::pair< Key, Value > data;
      BucketNode< Key, Value > * next;
    };
  }

  template< class Key, class Value, class Hash, class Equal >
  class HashTable;

  template< class Key, class Value >
  class HashConstIterator {
  public:
    const std::pair< Key, Value >& operator*() const
    {
      return node_->data;
    }

    const std::pair< Key, Value >* operator->() const
    {
      return std::addressof(node_->data);
    }

    HashConstIterator< Key, Value >& operator++()
    {
      node_ = node_->next;
      advance();
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
      return node_ == other.node_;
    }

    bool operator!=(const HashConstIterator< Key, Value >& other) const
    {
      return node_ != other.node_;
    }

  private:
    template< class K, class V, class H, class E >
    friend class HashTable;

    using Node = detail::BucketNode< Key, Value >;

    Node * const * buckets_;
    std::size_t bucketCount_;
    std::size_t bucket_;
    const Node * node_;

    HashConstIterator(Node * const * buckets, std::size_t count, std::size_t bucket, const Node * node):
      buckets_(buckets),
      bucketCount_(count),
      bucket_(bucket),
      node_(node)
    {}

    void advance()
    {
      while (node_ == nullptr && bucket_ + 1 < bucketCount_) {
        ++bucket_;
        node_ = buckets_[bucket_];
      }
    }
  };

  template< class Key, class Value, class Hash = std::hash< Key >, class Equal = std::equal_to< Key > >
  class HashTable {
  public:
    using const_iterator = HashConstIterator< Key, Value >;

    HashTable():
      buckets_(new Node *[default_bucket_count]()),
      bucketCount_(default_bucket_count),
      size_(0)
    {}

    explicit HashTable(std::size_t buckets):
      buckets_(new Node *[atLeastMin(buckets)]()),
      bucketCount_(atLeastMin(buckets)),
      size_(0)
    {}

    HashTable(const HashTable< Key, Value, Hash, Equal >& other):
      buckets_(new Node *[other.bucketCount_]()),
      bucketCount_(other.bucketCount_),
      size_(0),
      hash_(other.hash_),
      equal_(other.equal_)
    {
      try {
        for (const_iterator it = other.cbegin(); it != other.cend(); ++it) {
          add(it->first, it->second);
        }
      } catch (...) {
        clear();
        delete[] buckets_;
        throw;
      }
    }

    HashTable(HashTable< Key, Value, Hash, Equal >&& other) noexcept:
      buckets_(other.buckets_),
      bucketCount_(other.bucketCount_),
      size_(other.size_),
      hash_(other.hash_),
      equal_(other.equal_)
    {
      other.buckets_ = nullptr;
      other.bucketCount_ = 0;
      other.size_ = 0;
    }

    ~HashTable()
    {
      if (buckets_ != nullptr) {
        clear();
        delete[] buckets_;
      }
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
        HashTable< Key, Value, Hash, Equal > tmp(std::move(other));
        swap(tmp);
      }
      return *this;
    }

    Value& operator[](const Key& key)
    {
      const std::size_t bucket = hash_(key) % bucketCount_;
      for (Node * node = buckets_[bucket]; node != nullptr; node = node->next) {
        if (equal_(node->data.first, key)) {
          return node->data.second;
        }
      }
      return prepend(bucket, key, Value())->data.second;
    }

    void add(const Key& key, const Value& value)
    {
      const std::size_t bucket = hash_(key) % bucketCount_;
      for (Node * node = buckets_[bucket]; node != nullptr; node = node->next) {
        if (equal_(node->data.first, key)) {
          node->data.second = value;
          return;
        }
      }
      prepend(bucket, key, value);
    }

    Value& at(const Key& key)
    {
      Node * node = locate(key);
      if (node == nullptr) {
        throw std::out_of_range("key is missing");
      }
      return node->data.second;
    }

    const Value& at(const Key& key) const
    {
      const Node * node = locate(key);
      if (node == nullptr) {
        throw std::out_of_range("key is missing");
      }
      return node->data.second;
    }

    bool has(const Key& key) const
    {
      return locate(key) != nullptr;
    }

    const_iterator find(const Key& key) const
    {
      const std::size_t bucket = hash_(key) % bucketCount_;
      for (Node * node = buckets_[bucket]; node != nullptr; node = node->next) {
        if (equal_(node->data.first, key)) {
          return const_iterator(buckets_, bucketCount_, bucket, node);
        }
      }
      return cend();
    }

    Value drop(const Key& key)
    {
      const std::size_t bucket = hash_(key) % bucketCount_;
      Node * prev = nullptr;
      Node * node = buckets_[bucket];
      while (node != nullptr) {
        if (equal_(node->data.first, key)) {
          if (prev == nullptr) {
            buckets_[bucket] = node->next;
          } else {
            prev->next = node->next;
          }
          Value value = std::move(node->data.second);
          delete node;
          --size_;
          return value;
        }
        prev = node;
        node = node->next;
      }
      throw std::out_of_range("key is missing");
    }

    void rehash(std::size_t buckets)
    {
      const std::size_t newCount = atLeastMin(buckets);
      Node ** newBuckets = new Node *[newCount]();
      for (std::size_t b = 0; b < bucketCount_; ++b) {
        Node * node = buckets_[b];
        while (node != nullptr) {
          Node * next = node->next;
          const std::size_t target = hash_(node->data.first) % newCount;
          node->next = newBuckets[target];
          newBuckets[target] = node;
          node = next;
        }
      }
      delete[] buckets_;
      buckets_ = newBuckets;
      bucketCount_ = newCount;
    }

    std::size_t size() const
    {
      return size_;
    }

    std::size_t capacity() const
    {
      return bucketCount_;
    }

    bool empty() const
    {
      return size_ == 0;
    }

    const_iterator cbegin() const
    {
      std::size_t bucket = 0;
      const Node * node = bucketCount_ > 0 ? buckets_[0] : nullptr;
      while (node == nullptr && bucket + 1 < bucketCount_) {
        ++bucket;
        node = buckets_[bucket];
      }
      return const_iterator(buckets_, bucketCount_, bucket, node);
    }

    const_iterator cend() const
    {
      return const_iterator(buckets_, bucketCount_, bucketCount_, nullptr);
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
    using Node = detail::BucketNode< Key, Value >;

    static constexpr std::size_t min_bucket_count = 1;
    static constexpr std::size_t default_bucket_count = 16;

    Node ** buckets_;
    std::size_t bucketCount_;
    std::size_t size_;
    Hash hash_;
    Equal equal_;

    static std::size_t atLeastMin(std::size_t buckets)
    {
      return buckets < min_bucket_count ? min_bucket_count : buckets;
    }

    void swap(HashTable< Key, Value, Hash, Equal >& other) noexcept
    {
      std::swap(buckets_, other.buckets_);
      std::swap(bucketCount_, other.bucketCount_);
      std::swap(size_, other.size_);
      std::swap(hash_, other.hash_);
      std::swap(equal_, other.equal_);
    }

    Node * locate(const Key& key) const
    {
      const std::size_t bucket = hash_(key) % bucketCount_;
      for (Node * node = buckets_[bucket]; node != nullptr; node = node->next) {
        if (equal_(node->data.first, key)) {
          return node;
        }
      }
      return nullptr;
    }

    Node * prepend(std::size_t bucket, const Key& key, const Value& value)
    {
      Node * node = new Node{ std::make_pair(key, value), buckets_[bucket] };
      buckets_[bucket] = node;
      ++size_;
      return node;
    }

    void clear()
    {
      for (std::size_t b = 0; b < bucketCount_; ++b) {
        Node * node = buckets_[b];
        while (node != nullptr) {
          Node * next = node->next;
          delete node;
          node = next;
        }
        buckets_[b] = nullptr;
      }
      size_ = 0;
    }
  };

}

#endif
