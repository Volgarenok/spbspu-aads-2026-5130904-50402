#include "commandHandler.hpp"
#include "parserUtils.hpp"

#include <iostream>
#include <string>

int main()
{
  matveev::World world;
  std::string line;

  while (std::getline(std::cin, line))
  {
    matveev::List< std::string > tokens = matveev::splitLine(line);
    matveev::executeCommand(std::cout, world, tokens);
  }

  return 0;
}
