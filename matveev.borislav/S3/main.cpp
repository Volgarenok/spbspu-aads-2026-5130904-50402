#include "graph.hpp"

#include <iostream>

int main()
{
  matveev::Graph first;
  matveev::Graph second;

  first.bind("a", "b", 10);
  second.bind("a", "b", 20);
  second.bind("b", "c", 30);

  first.append(second);

  std::cout << first.hasVertex("c") << '\n';
  std::cout << first.hasEdge("a", "b") << '\n';
  std::cout << first.hasEdge("b", "c") << '\n';

  const matveev::List< unsigned long long >& weights = first.getWeights("a", "b");

  for (matveev::LCIter< unsigned long long > it = weights.begin(); it != weights.end(); ++it)
  {
    std::cout << *it << '\n';
  }

  return 0;
}
