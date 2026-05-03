#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP
#include <cstddef>
#include <utility>
#include "../common/Vector.hpp"
#include "../common/list.hpp"
#include "hashIters.hpp"
namespace karpovich
{

  template < class Key, class Value, class Hash, class Equal >
  class HashTable
  {
    friend class HashIter< Key, Value, Hash, Equal >;
    friend class HashConstIter< Key, Value, Hash, Equal >;
    using value_type = std::pair< Key, Value >;

  public:
    using HIter = HashIter< Key, Value, Hash, Equal >;
    using HCIter = HashConstIter< Key, Value, Hash, Equal >;

    HashTable(size_t slots);
    HashTable(std::initializer_list< std::pair< Key, Value > > il);
    ~HashTable();

    HashTable(const HashTable &other);
    HashTable(HashTable &&other) noexcept;
    HashTable &operator=(const HashTable &other);
    HashTable &operator=(HashTable &&other) noexcept;

    void add(Key k, Value v);
    Value drop(Key k);
    Value get(Key k) const;
    bool has(Key k) const noexcept;
    void rehash(size_t slots);

    void clear() noexcept;
    size_t size() const noexcept;
    bool empty() const noexcept;
    void swap(HashTable &other) noexcept;

    HIter begin();
    HIter end();
    HCIter begin() const;
    HCIter end() const;

  private:
    Vector< List< value_type > > data_;
    size_t capacity_;
    size_t size_;
    Hash hasher_;
    Equal comparator_;
  };
}

template < class Key, class Value, class Hash, class Equal >
karpovich::HashTable< Key, Value, Hash, Equal >::HashTable(size_t slots):
  data_(),
  hasher_(Hash{}),
  comparator_(Equal{}),
  capacity_(slots),
  size_(0)
{
  data_.reserve(slots);
}

template < class Key, class Value, class Hash, class Equal >
karpovich::HashTable< Key, Value, Hash, Equal >::HashTable(std::initializer_list< std::pair< Key, Value > > il):
  data_(),
  hasher_(Hash{}),
  comparator_(Equal{}),
  capacity_(il.size()),
  size_(0)
{
  data_.reserve(il.size());
  for (const std::pair< Key, Value > &v : il) {
    size_t id = hasher_(v.first) % size_;
    data_[id] = v.second;
  }
}

#endif
