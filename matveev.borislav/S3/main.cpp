#include "commandHandler.hpp"

#include <iostream>

int main()
{
  matveev::GraphCollection graphs;
  matveev::Graph graph;

  graph.bind("a", "b", 10);
  graph.bind("a", "c", 20);
  graph.bind("c", "a", 30);

  graphs.addGraph("old", graph);

  matveev::List< std::string > tokens;

  tokens = matveev::splitLine("extract new old 2 a c");
  matveev::executeCommand(std::cout, graphs, tokens);

  tokens = matveev::splitLine("outbound new a");
  matveev::executeCommand(std::cout, graphs, tokens);

  return 0;
}
