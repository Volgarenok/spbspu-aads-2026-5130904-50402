#include "graphCollection.hpp"

#include <iostream>

int main()
{
  matveev::GraphCollection graphs;
  matveev::Graph graph;

  graph.bind("a", "b", 10);
  graphs.addGraph("gr1", graph);

  std::cout << graphs.hasGraph("gr1") << '\n';
  std::cout << graphs.hasGraph("gr2") << '\n';
  std::cout << graphs.at("gr1").hasEdge("a", "b") << '\n';

  return 0;
}
