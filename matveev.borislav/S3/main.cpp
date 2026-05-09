#include "commandHandler.hpp"
#include "graphReader.hpp"
#include "parserUtils.hpp"

#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "<INVALID COMMAND>\n";
    return 1;
  }

  std::ifstream input(argv[1]);

  if (!input)
  {
    std::cerr << "<INVALID COMMAND>\n";
    return 1;
  }

  matveev::GraphCollection graphs;

  try
  {
    matveev::readGraphs(input, graphs);
  }
  catch (const std::exception&)
  {
    std::cerr << "<INVALID COMMAND>\n";
    return 1;
  }

  std::string line;

  while (std::getline(std::cin, line))
  {
    matveev::List< std::string > tokens = matveev::splitLine(line);
    matveev::executeCommand(std::cout, graphs, tokens);
  }

  return 0;
}
