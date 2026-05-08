#include "graphOutput.hpp"

#include <iostream>
#include <string>

int main()
{
  matveev::Graph graph;

  graph.bind("a", "c", 30);
  graph.bind("a", "b", 20);
  graph.bind("a", "b", 10);

  matveev::List< std::string > vertexes = matveev::collectVertexNames(graph);
  matveev::List< matveev::EdgeOutput > rows = matveev::collectOutboundEdges(graph, "a");

  matveev::printStringList(std::cout, vertexes);
  matveev::printEdgeRows(std::cout, rows);

  return 0;
}
