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
  matveev::HashTable< int, std::string, IntHash, IntEqual > first(3, 2);
  matveev::HashTable< int, std::string, IntHash, IntEqual > second(3, 2);

  first.add(1, "one");
  second.add(2, "two");

  first.swap(second);

  std::cout << first.at(2) << '\n';
  std::cout << second.at(1) << '\n';
  std::cout << first.size() << '\n';
  std::cout << second.size() << '\n';

  return 0;
}
