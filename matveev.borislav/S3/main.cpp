#include "graph.hpp"

#include <iostream>
#include <string>

int main()
{
  matveev::Graph graph;

  graph.bind("a", "b", 10);
  graph.bind("a", "c", 20);
  graph.bind("c", "a", 30);

  matveev::List< std::string > selected;
  selected.insertAfter(selected.beforeBegin(), "c");
  selected.insertAfter(selected.beforeBegin(), "a");

  matveev::Graph result = graph.extract(selected);

  std::cout << result.hasVertex("a") << '\n';
  std::cout << result.hasVertex("b") << '\n';
  std::cout << result.hasVertex("c") << '\n';
  std::cout << result.hasEdge("a", "c") << '\n';
  std::cout << result.hasEdge("a", "b") << '\n';
  std::cout << result.hasEdge("c", "a") << '\n';

  return 0;
}
