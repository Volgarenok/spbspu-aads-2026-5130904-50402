#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP
#include <cstddef>
#include <functional>
#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <utility>
#include "vector.hpp"
#include "list.hpp"
#include "hasher.hpp"

namespace permyakov
{
  template< class Key, class Value, class Hash, class Equal > class HIter;
  template< class Key, class Value, class Hash, class Equal > class HCIter;

  using HIt = HIter< Key, Value, Hash, Equal >;
  using HCIt = HCIter< Key, Value, Hash, Equal >;

  template< class Key, class Value, class Hash = sypHasher< Key >, class Equal = std::equal_to< Key > > class HashTable
  {
    Vector< List< std::pair< Key, Value > > > data_;
    Hash hasher_;
    Equal comparator_;
    size_t size_;
    size_t elements_;
    friend class HIt;
    friend class HCIt;
    public:
      explicit HashTable(size_t slots);
      HashTable(std::initializer_list< std::pair< Key, Value > > il);

      void add(Key k, Value val);
      void drop(Key k);
      void clear() noexcept;
      void swap(HashTable & other) noexcept;
      void rehash(size_t slots);
      bool has(Key k) const noexcept;
      bool isEmpty() const noexcept;
      size_t size() const noexcept;
      Value & get(Key k);
      const Value get(Key k) const;

      HIt begin() noexcept;
      HIt end() noexcept;
      HCIt cbegin() const noexcept;
      HCIt cend() const noexcept;
  }
}

#endif