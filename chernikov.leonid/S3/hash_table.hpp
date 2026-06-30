#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include "List.hpp"
#include <cstddef>
#include <stdexcept>
#include <functional>
#include <utility>

namespace chernikov {

  template < typename Key, typename Value, typename Hash = std::hash< Key >, typename Equal = std::equal_to< Key > >
  class HashTable
  {
  public:
    using key_type = Key;
    using mapped_type = Value;
    using value_type = std::pair< const Key, Value >;
    using size_type = std::size_t;

  private:
    using BucketValue = std::pair< Key, Value >;
    using Bucket = List< BucketValue >;

    Bucket *buckets_;
    size_type bucket_count_;
    size_type element_count_;
    size_type max_elements_;

    Hash hasher_;
    Equal equal_;

    size_type bucket_index(const Key &key) const
    {
      return hasher_(key) % bucket_count_;
    }

  public:
    explicit HashTable(size_type initial_size = 8):
      bucket_count_(initial_size > 0 ? initial_size : 1),
      element_count_(0),
      max_elements_(initial_size > 0 ? initial_size * 2 : 2)
    {
      buckets_ = new Bucket[bucket_count_];
    }

    ~HashTable()
    {
      delete[] buckets_;
    }

    HashTable(const HashTable &other):
      bucket_count_(other.bucket_count_),
      element_count_(other.element_count_),
      max_elements_(other.max_elements_)
    {
      buckets_ = new Bucket[bucket_count_];
      for (size_type i = 0; i < bucket_count_; ++i)
      {
        buckets_[i] = other.buckets_[i];
      }
    }

    HashTable(HashTable &&other) noexcept:
      buckets_(other.buckets_),
      bucket_count_(other.bucket_count_),
      element_count_(other.element_count_),
      max_elements_(other.max_elements_)
    {
      other.buckets_ = nullptr;
      other.bucket_count_ = 0;
      other.element_count_ = 0;
    }

    HashTable &operator=(const HashTable &other)
    {
      if (this != &other)
      {
        delete[] buckets_;
        bucket_count_ = other.bucket_count_;
        element_count_ = other.element_count_;
        max_elements_ = other.max_elements_;
        buckets_ = new Bucket[bucket_count_];
        for (size_type i = 0; i < bucket_count_; ++i)
        {
          buckets_[i] = other.buckets_[i];
        }
      }
      return *this;
    }

    HashTable &operator=(HashTable &&other) noexcept
    {
      if (this != &other)
      {
        delete[] buckets_;
        buckets_ = other.buckets_;
        bucket_count_ = other.bucket_count_;
        element_count_ = other.element_count_;
        max_elements_ = other.max_elements_;
        other.buckets_ = nullptr;
        other.bucket_count_ = 0;
        other.element_count_ = 0;
      }
      return *this;
    }

    // ==================== ОСНОВНЫЕ МЕТОДЫ ====================

    void add(const Key &k, const Value &v)
    {
      if (element_count_ >= max_elements_)
      {
        size_type index = bucket_index(k);
        Bucket &bucket = buckets_[index];

        for (auto it = bucket.begin(); it != bucket.end(); ++it)
        {
          if (equal_((*it).first, k))
          {
            (*it).second = v;
            return;
          }
        }

        throw std::overflow_error("HashTable overflow");
      }

      size_type index = bucket_index(k);
      Bucket &bucket = buckets_[index];

      for (auto it = bucket.begin(); it != bucket.end(); ++it)
      {
        if (equal_((*it).first, k))
        {
          (*it).second = v;
          return;
        }
      }

      bucket.add(BucketValue(k, v));
      ++element_count_;
    }

    Value drop(const Key &k)
    {
      size_type index = bucket_index(k);
      Bucket &bucket = buckets_[index];

      Bucket new_bucket;
      Value removed_value;
      bool found = false;

      for (auto it = bucket.begin(); it != bucket.end(); ++it)
      {
        if (!found && equal_((*it).first, k))
        {
          removed_value = (*it).second;
          found = true;
          --element_count_;
        } else
        {
          new_bucket.push_back(*it);
        }
      }

      if (!found)
      {
        throw std::out_of_range("Key not found");
      }

      bucket = std::move(new_bucket);
      return removed_value;
    }

    bool has(const Key &k) const
    {
      size_type index = bucket_index(k);
      const Bucket &bucket = buckets_[index];

      for (auto it = bucket.cbegin(); it != bucket.cend(); ++it)
      {
        if (equal_((*it).first, k))
        {
          return true;
        }
      }
      return false;
    }

    Value &get(const Key &k)
    {
      size_type index = bucket_index(k);
      Bucket &bucket = buckets_[index];

      for (auto it = bucket.begin(); it != bucket.end(); ++it)
      {
        if (equal_((*it).first, k))
        {
          return (*it).second;
        }
      }
      throw std::out_of_range("Key not found");
    }

    const Value &get(const Key &k) const
    {
      return const_cast< HashTable * >(this)->get(k);
    }

    void rehash(size_t slots)
    {
      if (slots == 0)
      {
        slots = 1;
      }

      Bucket *old_buckets = buckets_;
      size_type old_count = bucket_count_;

      bucket_count_ = slots;
      max_elements_ = slots * 2;
      buckets_ = new Bucket[bucket_count_];
      element_count_ = 0;

      for (size_type i = 0; i < old_count; ++i)
      {
        Bucket &old_bucket = old_buckets[i];
        for (auto it = old_bucket.begin(); it != old_bucket.end(); ++it)
        {
          size_type new_index = bucket_index((*it).first);
          buckets_[new_index].add(*it);
          ++element_count_;
        }
      }

      delete[] old_buckets;
    }

    void clear()
    {
      for (size_type i = 0; i < bucket_count_; ++i)
      {
        buckets_[i].clear();
      }
      element_count_ = 0;
    }

    // ==================== ИНФОРМАЦИОННЫЕ МЕТОДЫ ====================

    size_type size() const
    {
      return element_count_;
    }

    bool empty() const
    {
      return element_count_ == 0;
    }

    size_type bucket_count() const
    {
      return bucket_count_;
    }

    float load_factor() const
    {
      if (bucket_count_ == 0)
        return 0.0f;
      return static_cast< float >(element_count_) / bucket_count_;
    }

    void set_max_elements(size_type max)
    {
      max_elements_ = max;
    }

    Value &operator[](const Key &k)
    {
      if (!has(k))
      {
        add(k, Value());
      }
      return get(k);
    }

    // ==================== ИТЕРАТОР ====================

    class Iterator
    {
    private:
      const Bucket *buckets_;
      size_type bucket_count_;
      size_type current_bucket_;
      LCIter< BucketValue > bucket_iter_;
      bool is_end_;

      void find_next_valid()
      {
        while (current_bucket_ < bucket_count_)
        {
          bucket_iter_ = buckets_[current_bucket_].cbegin();
          if (bucket_iter_ != buckets_[current_bucket_].cend())
          {
            return;
          }
          ++current_bucket_;
        }
        is_end_ = true;
      }

    public:
      Iterator(const Bucket *buckets, size_type bucket_count, bool end = false):
        buckets_(buckets),
        bucket_count_(bucket_count),
        current_bucket_(0),
        is_end_(end)
      {
        if (!end && bucket_count_ > 0)
        {
          find_next_valid();
        }
      }

      const std::pair< const Key, Value > &operator*() const
      {
        return reinterpret_cast< const std::pair< const Key, Value > & >(*bucket_iter_);
      }

      Iterator &operator++()
      {
        if (is_end_)
          return *this;

        ++bucket_iter_;
        if (bucket_iter_ == buckets_[current_bucket_].cend())
        {
          ++current_bucket_;
          find_next_valid();
        }
        return *this;
      }

      bool operator==(const Iterator &other) const
      {
        if (is_end_ && other.is_end_)
          return true;
        if (is_end_ || other.is_end_)
          return false;
        return current_bucket_ == other.current_bucket_ && bucket_iter_ == other.bucket_iter_;
      }

      bool operator!=(const Iterator &other) const
      {
        return !(*this == other);
      }
    };

    Iterator begin() const
    {
      return Iterator(buckets_, bucket_count_);
    }

    Iterator end() const
    {
      return Iterator(buckets_, bucket_count_, true);
    }
  };

}

#endif