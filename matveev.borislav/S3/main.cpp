#include "hash-table.hpp"

#include <cstddef>
#include <iostream>
#include <string>

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
  matveev::HashTable< int, std::string, IntHash, IntEqual > table(5, 2);

  table.add(1, "one");
  table.add(6, "six");

  std::cout << table.has(1) << '\n';
  std::cout << table.has(2) << '\n';
  std::cout << table.at(6) << '\n';
  std::cout << table.size() << '\n';

  return 0;
}
