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

  std::cout << table.drop(3) << '\n';
  std::cout << table.has(3) << '\n';
  std::cout << table.size() << '\n';

  table.clear();

  std::cout << table.has(1) << '\n';
  std::cout << table.has(2) << '\n';
  std::cout << table.size() << '\n';

  return 0;
}
