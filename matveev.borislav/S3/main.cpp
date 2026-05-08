#include "parserUtils.hpp"

#include <iostream>

int main()
{
  unsigned long long weight = 0;
  size_t count = 0;

  std::cout << matveev::parseUnsignedLongLong("123", weight) << ' ' << weight << '\n';
  std::cout << matveev::parseUnsignedLongLong("12a", weight) << '\n';
  std::cout << matveev::parseSize("5", count) << ' ' << count << '\n';

  return 0;
}
