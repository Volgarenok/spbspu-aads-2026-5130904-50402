#include "commands.hpp"

#include <iostream>

int main()
{
  matveev::GraphCollection graphs;
  matveev::Graph graph;

  graphs.addGraph("ccc", graph);
  graphs.addGraph("aaa", graph);
  graphs.addGraph("bbb", graph);

  matveev::printGraphs(std::cout, graphs);

  return 0;
}
