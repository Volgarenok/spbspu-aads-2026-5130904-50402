#include "graphCollection.hpp"

#include <iostream>

int main()
{
  matveev::GraphCollection graphs;
  matveev::Graph first;
  matveev::Graph second;

  first.bind("a", "b", 10);
  second.bind("a", "b", 20);
  second.bind("b", "c", 30);

  graphs.addGraph("first", first);
  graphs.addGraph("second", second);

  graphs.mergeGraphs("third", "first", "second");

  std::cout << graphs.hasGraph("third") << '\n';
  std::cout << graphs.at("third").hasVertex("c") << '\n';
  std::cout << graphs.at("third").hasEdge("a", "b") << '\n';
  std::cout << graphs.at("third").hasEdge("b", "c") << '\n';

  const matveev::List< unsigned long long >& weights = graphs.at("third").getWeights("a", "b");

  for (matveev::LCIter< unsigned long long > it = weights.begin(); it != weights.end(); ++it)
  {
    std::cout << *it << '\n';
  }

  return 0;
}
