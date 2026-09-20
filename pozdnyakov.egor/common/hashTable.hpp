#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <cstddef>
#include <functional>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <utility>
#include "list.hpp"
#include "vector.hpp"

namespace pozdnyakov
{
  namespace detail
  {
    constexpr std::size_t SMALL_TABLE_SLOTS = 10;
    constexpr std::size_t INITIAL_TABLE_SLOTS = 20;
    constexpr std::size_t DEFAULT_MAX_CHAIN = 10;
    constexpr float DEFAULT_MAX_LOAD_FACTOR = 1.0f;

    template< class Key >
    struct DefaultEqual
    {
      bool operator()(const Key &lhs, const Key &rhs) const
      {
        return lhs == rhs;
      }
    };

    struct DefaultSlotUpdater
    {
      std::size_t operator()(std::size_t before) const
      {
        return (before < SMALL_TABLE_SLOTS) ? INITIAL_TABLE_SLOTS : before * 2;
      }
    };
  }

  template< class Key, class Value, class Hash, class Equal = detail::DefaultEqual< Key > >
  class HashTable
  {
  public:
    using value_type = std::pair< Key, Value >;

    class iterator
    {
      friend class HashTable;

    public:
      using iterator_category = std::forward_iterator_tag;
      using value_type = std::pair< Key, Value >;
      using difference_type = std::ptrdiff_t;
      using pointer = std::pair< Key, Value > *;
      using reference = std::pair< Key, Value > &;

      iterator() noexcept:
        bucketsPtr_(nullptr),
        bucketIdx_(0),
        listIt_()
      {}

      reference operator*() const
      {
        return *listIt_;
      }

      pointer operator->() const
      {
        return std::addressof(*listIt_);
      }

      iterator &operator++()
      {
        ++listIt_;
        skipEmpty();
        return *this;
      }

      iterator operator++(int)
      {
        iterator temp = *this;
        ++(*this);
        return temp;
      }

      bool operator==(const iterator &other) const
      {
        if (bucketsPtr_ != other.bucketsPtr_) {
          return false;
        }
        if (bucketsPtr_ == nullptr) {
          return true;
        }
        if (bucketIdx_ != other.bucketIdx_) {
          return false;
        }
        if (bucketIdx_ == bucketsPtr_->size()) {
          return true;
        }
        return listIt_ == other.listIt_;
      }

      bool operator!=(const iterator &other) const
      {
        return !(*this == other);
      }

    private:
      Vector< List< std::pair< Key, Value > > > *bucketsPtr_;
      std::size_t bucketIdx_;
      LIter< std::pair< Key, Value > > listIt_;

      iterator(Vector< List< std::pair< Key, Value > > > *bPtr, std::size_t idx,
        LIter< std::pair< Key, Value > > it):
        bucketsPtr_(bPtr),
        bucketIdx_(idx),
        listIt_(it)
      {
        skipEmpty();
      }

      void skipEmpty()
      {
        if (bucketsPtr_ == nullptr) {
          return;
        }
        while (bucketIdx_ < bucketsPtr_->size()) {
          if (listIt_ != (*bucketsPtr_)[bucketIdx_].end()) {
            return;
          }
          ++bucketIdx_;
          if (bucketIdx_ < bucketsPtr_->size()) {
            listIt_ = (*bucketsPtr_)[bucketIdx_].begin();
          }
        }
        bucketIdx_ = bucketsPtr_->size();
        listIt_ = LIter< std::pair< Key, Value > >();
      }
    };

    HashTable():
      buckets_(),
      bucketSizes_(),
      size_(0),
      hashObj_(),
      equalObj_(),
      maxLoadFactor_(detail::DEFAULT_MAX_LOAD_FACTOR),
      maxBucketSizeLimit_(detail::DEFAULT_MAX_CHAIN),
      slotUpdater_(detail::DefaultSlotUpdater())
    {}

    void add(Key k, Value v)
    {
      if (buckets_.size() == 0) {
        rehash(slotUpdater_(0));
      }

      const std::size_t idx = hashObj_(k) % buckets_.size();
      for (auto it = buckets_[idx].begin(); it != buckets_[idx].end(); ++it) {
        if (equalObj_((*it).first, k)) {
          (*it).second = std::move(v);
          return;
        }
      }

      buckets_[idx].pushFront(std::make_pair(std::move(k), std::move(v)));
      ++bucketSizes_[idx];
      ++size_;

      if (loadFactor() > maxLoadFactor_ || bucketSizes_[idx] > maxBucketSizeLimit_) {
        rehash(slotUpdater_(buckets_.size()));
      }
    }

    bool drop(const Key &k)
    {
      if (buckets_.size() == 0) {
        return false;
      }

      const std::size_t idx = hashObj_(k) % buckets_.size();
      List< std::pair< Key, Value > > &chain = buckets_[idx];

      auto prev = chain.end();
      auto it = chain.begin();
      while (it != chain.end()) {
        if (equalObj_((*it).first, k)) {
          chain.eraseAfter(prev);
          --bucketSizes_[idx];
          --size_;
          return true;
        }
        prev = it;
        ++it;
      }
      return false;
    }

    Value *find(const Key &k)
    {
      if (buckets_.size() == 0) {
        return nullptr;
      }
      const std::size_t idx = hashObj_(k) % buckets_.size();
      for (auto it = buckets_[idx].begin(); it != buckets_[idx].end(); ++it) {
        if (equalObj_((*it).first, k)) {
          return std::addressof((*it).second);
        }
      }
      return nullptr;
    }

    const Value *find(const Key &k) const
    {
      if (buckets_.size() == 0) {
        return nullptr;
      }
      const std::size_t idx = hashObj_(k) % buckets_.size();
      for (auto it = buckets_[idx].cbegin(); it != buckets_[idx].cend(); ++it) {
        if (equalObj_((*it).first, k)) {
          return std::addressof((*it).second);
        }
      }
      return nullptr;
    }

    bool has(const Key &k) const
    {
      return find(k) != nullptr;
    }

    Value &at(const Key &k)
    {
      Value * const found = find(k);
      if (found == nullptr) {
        throw std::out_of_range("<KEY NOT FOUND>");
      }
      return *found;
    }

    const Value &at(const Key &k) const
    {
      const Value * const found = find(k);
      if (found == nullptr) {
        throw std::out_of_range("<KEY NOT FOUND>");
      }
      return *found;
    }

    void rehash(std::size_t slots)
    {
      if (slots == 0) {
        slots = 1;
      }
      Vector< List< std::pair< Key, Value > > > newBuckets;
      newBuckets.resize(slots);
      Vector< std::size_t > newBucketSizes;
      newBucketSizes.resize(slots);

      for (std::size_t i = 0; i < buckets_.size(); ++i) {
        for (auto it = buckets_[i].begin(); it != buckets_[i].end(); ++it) {
          const std::size_t idx = hashObj_((*it).first) % slots;
          newBuckets[idx].pushFront(std::move(*it));
          ++newBucketSizes[idx];
        }
      }

      buckets_ = std::move(newBuckets);
      bucketSizes_ = std::move(newBucketSizes);
    }

    void clear()
    {
      buckets_.clear();
      bucketSizes_.clear();
      size_ = 0;
    }

    float loadFactor() const
    {
      if (buckets_.size() == 0) {
        return 0.0f;
      }
      return static_cast< float >(size_) / static_cast< float >(buckets_.size());
    }

    std::size_t maxBucketSize() const
    {
      std::size_t maximum = 0;
      for (std::size_t i = 0; i < bucketSizes_.size(); ++i) {
        if (bucketSizes_[i] > maximum) {
          maximum = bucketSizes_[i];
        }
      }
      return maximum;
    }

    void maxLoadFactor(float mlf)
    {
      maxLoadFactor_ = mlf;
    }

    float maxLoadFactor() const noexcept
    {
      return maxLoadFactor_;
    }

    void maxBucketSizeLimit(std::size_t mbs)
    {
      maxBucketSizeLimit_ = mbs;
    }

    std::size_t maxBucketSizeLimit() const noexcept
    {
      return maxBucketSizeLimit_;
    }

    void setSlotUpdater(std::function< std::size_t(std::size_t) > updater)
    {
      slotUpdater_ = updater;
    }

    std::size_t size() const noexcept
    {
      return size_;
    }

    std::size_t slots() const noexcept
    {
      return buckets_.size();
    }

    bool empty() const noexcept
    {
      return size_ == 0;
    }

    iterator begin()
    {
      if (buckets_.size() == 0) {
        return end();
      }
      return iterator(std::addressof(buckets_), 0, buckets_[0].begin());
    }

    iterator end()
    {
      return iterator(std::addressof(buckets_), buckets_.size(), LIter< std::pair< Key, Value > >());
    }

  private:
    Vector< List< std::pair< Key, Value > > > buckets_;
    Vector< std::size_t > bucketSizes_;
    std::size_t size_;
    Hash hashObj_;
    Equal equalObj_;

    float maxLoadFactor_;
    std::size_t maxBucketSizeLimit_;
    std::function< std::size_t(std::size_t) > slotUpdater_;
  };
}

#endif
