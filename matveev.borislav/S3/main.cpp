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
  matveev::HashTable< int, std::string, SameHash, IntEqual > first(1, 2);

  first.add(1, "one");
  first.add(2, "two");
  first.add(3, "three");

  matveev::HashTable< int, std::string, SameHash, IntEqual > second(first);

  std::cout << second.at(1) << '\n';
  std::cout << second.at(2) << '\n';
  std::cout << second.at(3) << '\n';
  std::cout << second.size() << '\n';

  matveev::HashTable< int, std::string, SameHash, IntEqual > third(3, 3);
  third = first;

  std::cout << third.at(3) << '\n';
  std::cout << third.bucketCount() << '\n';
  std::cout << third.bucketCapacity() << '\n';

  return 0;
}
