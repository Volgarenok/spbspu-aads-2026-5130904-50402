#include "commands.hpp"

#include <iostream>

int main()
{
  matveev::Graph graph;

  graph.bind("a", "c", 30);
  graph.bind("a", "b", 20);
  graph.bind("a", "b", 10);

  matveev::printOutbound(std::cout, graph, "a");

  return 0;
}
