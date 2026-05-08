#include "graphOutput.hpp"

#include <iostream>
#include <string>

int main()
{
  matveev::GraphCollection graphs;
  matveev::Graph graph;

  graphs.addGraph("ccc", graph);
  graphs.addGraph("aaa", graph);
  graphs.addGraph("bbb", graph);

  matveev::List< std::string > names = matveev::collectGraphNames(graphs);

  for (matveev::LIter< std::string > it = names.begin(); it != names.end(); ++it)
  {
    std::cout << *it << '\n';
  }

  return 0;
}
