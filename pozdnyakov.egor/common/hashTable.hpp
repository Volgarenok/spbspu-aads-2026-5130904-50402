#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <functional>
#include <stdexcept>
#include <utility>
#include "list.hpp"
#include "vector.hpp"

namespace pozdnyakov
{
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
      return (before < 10) ? 20 : before * 2;
    }
  };

  template< class Key, class Value, class Hash, class Equal = DefaultEqual< Key > >
  class HashTable
  {
  private:
    Vector< List< std::pair< Key, Value > > > buckets_;
    Vector< std::size_t > bucketSizes_;
    std::size_t size_;
    Hash hashObj_;
    Equal equalObj_;

    float maxLoadFactor_;
    std::size_t maxBucketSizeLimit_;
    std::function< std::size_t(std::size_t) > slotUpdater_;

  public:
    class iterator
    {
      friend class HashTable;

    private:
      Vector< List< std::pair< Key, Value > > > *bucketsPtr_;
      std::size_t bucketIdx_;
      LIter< std::pair< Key, Value > > listIt_;

      void skipEmpty()
      {
        if (!bucketsPtr_) {
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
      }

      iterator(Vector< List< std::pair< Key, Value > > > *bPtr, std::size_t idx, LIter< std::pair< Key, Value > > it):
        bucketsPtr_(bPtr),
        bucketIdx_(idx),
        listIt_(it)
      {
        skipEmpty();
      }

    public:
      using iterator_category = std::forward_iterator_tag;
      using value_type = std::pair< Key, Value >;
      using difference_type = std::ptrdiff_t;
      using pointer = std::pair< Key, Value > *;
      using reference = std::pair< Key, Value > &;

      iterator():
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
        return &(*listIt_);
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
        if (bucketIdx_ == bucketsPtr_->size() && other.bucketIdx_ == other.bucketsPtr_->size()) {
          return true;
        }
        return bucketIdx_ == other.bucketIdx_ && listIt_ == other.listIt_;
      }

      bool operator!=(const iterator &other) const
      {
        return !(*this == other);
      }
    };

    HashTable():
      size_(0),
      maxLoadFactor_(1.0f),
      maxBucketSizeLimit_(10),
      slotUpdater_(DefaultSlotUpdater())
    {}

    float loadFactor() const
    {
      if (buckets_.size() == 0) {
        return 0.0f;
      }
      return static_cast< float >(size_) / static_cast< float >(buckets_.size());
    }

    std::size_t maxBucketSize() const
    {
      std::size_t m = 0;
      for (std::size_t i = 0; i < bucketSizes_.size(); ++i) {
        if (bucketSizes_[i] > m) {
          m = bucketSizes_[i];
        }
      }
      return m;
    }

    void maxLoadFactor(float mlf)
    {
      maxLoadFactor_ = mlf;
    }

    float maxLoadFactor() const
    {
      return maxLoadFactor_;
    }

    void maxBucketSizeLimit(std::size_t mbs)
    {
      maxBucketSizeLimit_ = mbs;
    }

    std::size_t maxBucketSizeLimit() const
    {
      return maxBucketSizeLimit_;
    }

    void setSlotUpdater(std::function< std::size_t(std::size_t) > updater)
    {
      slotUpdater_ = updater;
    }

    void add(Key k, Value v)
    {
      if (buckets_.size() == 0) {
        rehash(slotUpdater_(0));
      }

      std::size_t idx = hashObj_(k) % buckets_.size();

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

      std::size_t idx = hashObj_(k) % buckets_.size();

      List< std::pair< Key, Value > > newList;
      bool found = false;

      for (auto it = buckets_[idx].begin(); it != buckets_[idx].end(); ++it) {
        if (equalObj_((*it).first, k)) {
          found = true;
        } else {
          newList.pushFront(*it);
        }
      }

      if (found) {
        buckets_[idx] = std::move(newList);
        --bucketSizes_[idx];
        --size_;
      }

      return found;
    }

    bool has(Key k)
    {
      if (buckets_.size() == 0) {
        return false;
      }
      std::size_t idx = hashObj_(k) % buckets_.size();
      for (auto it = buckets_[idx].begin(); it != buckets_[idx].end(); ++it) {
        if (equalObj_((*it).first, k)) {
          return true;
        }
      }
      return false;
    }

    Value &at(Key k)
    {
      if (buckets_.size() == 0) {
        throw std::runtime_error("Key not found");
      }
      std::size_t idx = hashObj_(k) % buckets_.size();
      for (auto it = buckets_[idx].begin(); it != buckets_[idx].end(); ++it) {
        if (equalObj_((*it).first, k)) {
          return (*it).second;
        }
      }
      throw std::runtime_error("Key not found");
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
          std::size_t idx = hashObj_((*it).first) % slots;
          newBuckets[idx].pushFront(std::move(*it));
          ++newBucketSizes[idx];
        }
      }

      buckets_ = std::move(newBuckets);
      bucketSizes_ = std::move(newBucketSizes);
    }

    std::size_t size() const
    {
      return size_;
    }

    bool empty() const
    {
      return size_ == 0;
    }

    iterator begin()
    {
      if (buckets_.size() == 0) {
        return end();
      }
      return iterator(&buckets_, 0, buckets_[0].begin());
    }

    iterator end()
    {
      return iterator(&buckets_, buckets_.size(), LIter< std::pair< Key, Value > >());
    }
  };
}

#endif
