#include "graph.hpp"

#include <iostream>

int main()
{
  matveev::Graph graph;

  graph.addVertex("a");
  graph.addVertex("b");
  graph.addVertex("a");

  std::cout << graph.hasVertex("a") << '\n';
  std::cout << graph.hasVertex("b") << '\n';
  std::cout << graph.hasVertex("c") << '\n';

  return 0;
}
