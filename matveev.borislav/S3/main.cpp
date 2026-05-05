#include "hash-table.hpp"

#include <cstddef>
#include <iostream>
#include <string>

namespace
{
struct SameHash
{
  size_t operator()(int) const
  {
    return 0;
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
  matveev::HashTable< int, std::string, SameHash, IntEqual > table(1, 2);

  table.add(1, "one");
  table.add(2, "two");
  table.add(3, "three");

  for (auto it = table.begin(); it != table.end(); ++it)
  {
    std::cout << it->key << ' ' << it->value << '\n';
  }

  return 0;
}
