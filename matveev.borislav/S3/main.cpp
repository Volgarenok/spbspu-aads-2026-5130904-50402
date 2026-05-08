#include "commandHandler.hpp"

#include <iostream>

int main()
{
  matveev::GraphCollection graphs;
  matveev::Graph graph;

  graph.addVertex("bbb");
  graph.addVertex("aaa");

  graphs.addGraph("gr1", graph);

  matveev::List< std::string > tokens = matveev::splitLine("vertexes gr1");
  matveev::executeCommand(std::cout, graphs, tokens);

  tokens = matveev::splitLine("vertexes gr2");
  matveev::executeCommand(std::cout, graphs, tokens);

  return 0;
}
