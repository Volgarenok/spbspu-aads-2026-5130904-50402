#include "graphCollection.hpp"

#include <iostream>
#include <string>

int main()
{
  matveev::GraphCollection graphs;
  matveev::Graph graph;

  graph.bind("a", "b", 10);
  graph.bind("a", "c", 20);
  graph.bind("c", "a", 30);

  graphs.addGraph("old", graph);

  matveev::List< std::string > vertexes;
  vertexes.insertAfter(vertexes.beforeBegin(), "c");
  vertexes.insertAfter(vertexes.beforeBegin(), "a");

  graphs.extractGraph("new", "old", vertexes);

  std::cout << graphs.hasGraph("new") << '\n';
  std::cout << graphs.at("new").hasVertex("a") << '\n';
  std::cout << graphs.at("new").hasVertex("b") << '\n';
  std::cout << graphs.at("new").hasVertex("c") << '\n';
  std::cout << graphs.at("new").hasEdge("a", "c") << '\n';
  std::cout << graphs.at("new").hasEdge("a", "b") << '\n';
  std::cout << graphs.at("new").hasEdge("c", "a") << '\n';

  return 0;
}
