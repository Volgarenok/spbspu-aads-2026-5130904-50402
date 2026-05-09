#include "commandHandler.hpp"

#include <iostream>

int main()
{
  matveev::GraphCollection graphs;
  matveev::Graph graph;

  graph.bind("a", "b", 10);
  graph.bind("a", "b", 20);
  graphs.addGraph("gr1", graph);

  matveev::List< std::string > tokens;

  tokens = matveev::splitLine("cut gr1 a b 20");
  matveev::executeCommand(std::cout, graphs, tokens);

  tokens = matveev::splitLine("outbound gr1 a");
  matveev::executeCommand(std::cout, graphs, tokens);

  tokens = matveev::splitLine("cut gr1 a b 30");
  matveev::executeCommand(std::cout, graphs, tokens);

  return 0;
}
