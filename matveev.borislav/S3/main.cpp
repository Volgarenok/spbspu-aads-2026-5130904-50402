#include "graph.hpp"
#include "hash-table.hpp"

#include <iostream>

int main()
{
  matveev::HashTable< matveev::EdgeKey, int, matveev::EdgeHash, matveev::EdgeEqual > table(5, 2);

  table.add(matveev::EdgeKey("a", "b"), 10);
  table.add(matveev::EdgeKey("b", "a"), 20);

  std::cout << table.at(matveev::EdgeKey("a", "b")) << '\n';
  std::cout << table.at(matveev::EdgeKey("b", "a")) << '\n';
  std::cout << table.has(matveev::EdgeKey("a", "a")) << '\n';

  return 0;
}
