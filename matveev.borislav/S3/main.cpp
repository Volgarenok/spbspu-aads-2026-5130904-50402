#include "commands.hpp"

#include <iostream>

int main()
{
  matveev::Graph graph;

  graph.addVertex("ccc");
  graph.addVertex("aaa");
  graph.addVertex("bbb");

  matveev::printVertexes(std::cout, graph);

  return 0;
}
