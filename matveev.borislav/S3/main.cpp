#include "commandHandler.hpp"

#include <iostream>

int main()
{
  matveev::GraphCollection graphs;
  matveev::List< std::string > tokens;

  tokens = matveev::splitLine("create gr1 3 c a b");
  matveev::executeCommand(std::cout, graphs, tokens);

  tokens = matveev::splitLine("vertexes gr1");
  matveev::executeCommand(std::cout, graphs, tokens);

  tokens = matveev::splitLine("create gr1 0");
  matveev::executeCommand(std::cout, graphs, tokens);

  return 0;
}
