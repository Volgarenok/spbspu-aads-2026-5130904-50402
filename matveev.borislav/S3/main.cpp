#include "graphOutput.hpp"

#include <iostream>
#include <string>

int main()
{
  matveev::Graph graph;

  graph.bind("a", "c", 30);
  graph.bind("a", "b", 20);
  graph.bind("a", "b", 10);
  graph.bind("c", "a", 40);

  matveev::List< matveev::EdgeOutput > rows = matveev::collectOutboundEdges(graph, "a");

  for (matveev::LIter< matveev::EdgeOutput > it = rows.begin(); it != rows.end(); ++it)
  {
    std::cout << it->vertex;

    for (matveev::LIter< unsigned long long > weight = it->weights.begin(); weight != it->weights.end(); ++weight)
    {
      std::cout << ' ' << *weight;
    }

    std::cout << '\n';
  }

  return 0;
}
