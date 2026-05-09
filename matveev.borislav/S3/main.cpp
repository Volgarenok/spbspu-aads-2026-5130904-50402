#include "commandHandler.hpp"

#include <iostream>

int main()
{
  matveev::GraphCollection graphs;
  matveev::Graph first;
  matveev::Graph second;

  first.bind("a", "b", 10);
  second.bind("a", "b", 20);
  second.bind("b", "c", 30);

  graphs.addGraph("first", first);
  graphs.addGraph("second", second);

  matveev::List< std::string > tokens;

  tokens = matveev::splitLine("merge third first second");
  matveev::executeCommand(std::cout, graphs, tokens);

  tokens = matveev::splitLine("outbound third a");
  matveev::executeCommand(std::cout, graphs, tokens);

  return 0;
}
