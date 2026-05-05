#include "hash-table.hpp"
#include "hashFunctions.hpp"

#include <iostream>
#include <string>

int main()
{
  matveev::HashTable< std::string, int, matveev::StringHash, matveev::StringEqual > table(5, 2);

  table.add("first", 10);
  table.add("second", 20);

  std::cout << table.at("first") << '\n';
  std::cout << table.at("second") << '\n';
  std::cout << table.has("third") << '\n';

  return 0;
}
