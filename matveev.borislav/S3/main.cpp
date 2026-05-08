#include "commands.hpp"

#include <iostream>

int main()
{
  matveev::Graph graph;

  graph.bind("c", "a", 30);
  graph.bind("b", "a", 20);
  graph.bind("b", "a", 10);

  matveev::printInbound(std::cout, graph, "a");

  return 0;
}
