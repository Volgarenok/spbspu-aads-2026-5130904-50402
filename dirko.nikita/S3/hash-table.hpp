#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP
#include <cstddef>
#include <utility>
#include "../common/list.hpp"
#include "../common/vector.hpp"

namespace dirko
{
  template < class Key, class Value, class Hash, class Equal >
  class HashTable
  {
  public:
    HashTable(size_t slots);

    void add(Key k, Value v);
    void drop(Key k);
    Value get(Key k) const;
    bool has(Key k) const noexcept;
    void rehash(size_t slots);

    void clear() noexcept;
    size_t size() const noexcept;
    bool empty() const noexcept;
    void swap(HashTable &other) noexcept;

  private:
    Vector< List< std::pair< Key, Value > > * > data_;
    Hash hasher_;
    Equal comparator_;
    size_t slots_;
    size_t elements_;
  };
}

template < class Key, class Value, class Hash, class Equal >
dirko::HashTable< Key, Value, Hash, Equal >::HashTable(size_t slots):
  data_(),
  hasher_(Hash{}),
  comparator_(Equal{}),
  slots_(slots),
  elements_(0)
{}
#endif
