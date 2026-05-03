#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP
#include <cstddef>
#include <utility>
#include "../common/list.hpp"
#include "../common/Vector.hpp"
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

    HashTable(std::size_t slots);
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
    Vector< List< value_type > > *data_;
    size_t capacity_;
    size_t size_;
    Hash hasher_;
    Equal comparator_;
  };
}
#endif
