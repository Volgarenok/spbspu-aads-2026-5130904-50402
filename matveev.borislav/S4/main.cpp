#include "dictionary-commands.hpp"
#include "dictionary-reader.hpp"

#include <exception>
#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << matveev::INVALID_COMMAND << '\n';
    return 1;
  }

  std::ifstream input(argv[1]);

  if (!input)
  {
    std::cerr << matveev::INVALID_COMMAND << '\n';
    return 1;
  }

  matveev::DictionaryStorage storage;

  try
  {
    storage = matveev::readDictionaries(input);
  }
  catch (const std::exception&)
  {
    std::cerr << matveev::INVALID_COMMAND << '\n';
    return 1;
  }

  std::string line;

  while (std::getline(std::cin, line))
  {
    matveev::executeCommand(std::cout, storage, line);
  }

  return 0;
}
