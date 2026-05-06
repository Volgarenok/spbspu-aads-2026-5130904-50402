#include "graph.hpp"

#include <iostream>

int main()
{
  matveev::Graph graph;

  graph.bind("a", "b", 10);
  graph.bind("a", "b", 20);
  graph.cut("a", "b", 20);

  std::cout << graph.hasVertex("a") << '\n';
  std::cout << graph.hasVertex("b") << '\n';
  std::cout << graph.hasEdge("a", "b") << '\n';

  const matveev::List< unsigned long long >& weights = graph.getWeights("a", "b");

  for (matveev::LCIter< unsigned long long > it = weights.begin(); it != weights.end(); ++it)
  {
    std::cout << *it << '\n';
  }

  graph.cut("a", "b", 10);
  std::cout << graph.hasEdge("a", "b") << '\n';

  return 0;
}
