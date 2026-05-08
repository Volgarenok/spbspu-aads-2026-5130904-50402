#include "graphCollection.hpp"

#include <iostream>

int main()
{
  matveev::GraphCollection graphs;
  matveev::Graph graph;

  graph.addVertex("a");
  graphs.addGraph("gr1", graph);

  for (auto it = graphs.graphs().cbegin(); it != graphs.graphs().cend(); ++it)
  {
    std::cout << it->key << '\n';
  }

  return 0;
}
