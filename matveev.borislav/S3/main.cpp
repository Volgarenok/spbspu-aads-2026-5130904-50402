#include "commandHandler.hpp"

#include <iostream>

int main()
{
  matveev::GraphCollection graphs;
  matveev::Graph graph;

  graph.bind("a", "c", 30);
  graph.bind("a", "b", 20);
  graph.bind("a", "b", 10);

  graphs.addGraph("gr1", graph);

  matveev::List< std::string > tokens = matveev::splitLine("outbound gr1 a");
  for (matveev::LIter< std::string > it = tokens.begin(); it != tokens.end(); ++it)
  {
    std::cout << "[" << *it << "]\n";
  }
  std::cout << graphs.hasGraph("gr1") << '\n';
  std::cout << graphs.at("gr1").hasVertex("a") << '\n';
  std::cout << graphs.at("gr1").hasEdge("a", "b") << '\n';
  std::cout << graphs.at("gr1").hasEdge("a", "c") << '\n';
  matveev::executeCommand(std::cout, graphs, tokens);

  tokens = matveev::splitLine("outbound gr1 z");
  matveev::executeCommand(std::cout, graphs, tokens);

  return 0;
}
