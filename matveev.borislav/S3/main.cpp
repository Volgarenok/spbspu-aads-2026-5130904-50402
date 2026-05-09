#include "commandHandler.hpp"

#include <iostream>

int main()
{
  matveev::GraphCollection graphs;
  matveev::Graph graph;

  graph.bind("c", "a", 30);
  graph.bind("b", "a", 20);
  graph.bind("b", "a", 10);

  graphs.addGraph("gr1", graph);

  matveev::List< std::string > tokens = matveev::splitLine("inbound gr1 a");
  matveev::executeCommand(std::cout, graphs, tokens);

  tokens = matveev::splitLine("inbound gr1 z");
  matveev::executeCommand(std::cout, graphs, tokens);

  return 0;
}
