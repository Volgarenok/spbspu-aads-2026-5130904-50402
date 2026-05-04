#include "hash-table.hpp"

#include <cstddef>
#include <iostream>

namespace
{
struct IntHash
{
  size_t operator()(int value) const
  {
    return static_cast< size_t >(value);
  }
};

struct IntEqual
{
  bool operator()(int lhs, int rhs) const
  {
    return lhs == rhs;
  }
};
}

int main()
{
  matveev::HashTable< int, int, IntHash, IntEqual > table(5, 2);

  std::cout << table.size() << '\n';
  std::cout << table.bucketCount() << '\n';
  std::cout << table.bucketCapacity() << '\n';

  return 0;
}
