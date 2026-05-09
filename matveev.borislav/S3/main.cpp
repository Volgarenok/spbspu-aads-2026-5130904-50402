#include "graphReader.hpp"
#include "commandHandler.hpp"

#include <fstream>
#include <iostream>

int main()
{
  std::ifstream input("test.txt");
  matveev::GraphCollection graphs;

  matveev::readGraphs(input, graphs);

  matveev::List< std::string > tokens = matveev::splitLine("outbound gr1 c");
  matveev::executeCommand(std::cout, graphs, tokens);

  return 0;
}
