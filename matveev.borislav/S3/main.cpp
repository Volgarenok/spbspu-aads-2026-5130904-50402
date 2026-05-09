#include "commandHandler.hpp"

#include <iostream>

int main()
{
  matveev::GraphCollection graphs;
  matveev::Graph graph;

  graphs.addGraph("gr1", graph);

  matveev::List< std::string > tokens;

  tokens = matveev::splitLine("bind gr1 a b 10");
  matveev::executeCommand(std::cout, graphs, tokens);

  tokens = matveev::splitLine("outbound gr1 a");
  matveev::executeCommand(std::cout, graphs, tokens);

  tokens = matveev::splitLine("bind gr1 a b x");
  matveev::executeCommand(std::cout, graphs, tokens);

  return 0;
}
