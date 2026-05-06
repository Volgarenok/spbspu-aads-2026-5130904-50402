#include "graph.hpp"

#include <iostream>

int main()
{
  matveev::Graph graph;

  graph.bind("a", "b", 10);
  graph.bind("a", "c", 20);

  std::cout << graph.vertexes().size() << '\n';
  std::cout << graph.edges().size() << '\n';

  return 0;
}
