#include "graphCollection.hpp"

#include <iostream>
#include <string>

int main()
{
  matveev::GraphCollection graphs;
  matveev::List< std::string > vertexes;

  vertexes.insertAfter(vertexes.beforeBegin(), "b");
  vertexes.insertAfter(vertexes.beforeBegin(), "a");

  graphs.createGraph("gr1", vertexes);

  std::cout << graphs.hasGraph("gr1") << '\n';
  std::cout << graphs.at("gr1").hasVertex("a") << '\n';
  std::cout << graphs.at("gr1").hasVertex("b") << '\n';
  std::cout << graphs.at("gr1").hasVertex("c") << '\n';

  return 0;
}
