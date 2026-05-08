#include "graphOutput.hpp"

#include <iostream>
#include <string>

int main()
{
  matveev::Graph graph;

  graph.addVertex("ccc");
  graph.addVertex("aaa");
  graph.addVertex("bbb");

  matveev::List< std::string > names = matveev::collectVertexNames(graph);

  for (matveev::LIter< std::string > it = names.begin(); it != names.end(); ++it)
  {
    std::cout << *it << '\n';
  }

  return 0;
}
