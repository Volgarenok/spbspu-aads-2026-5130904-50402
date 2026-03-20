#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "parser.hpp"

int main(int argc, char* argv[])
{
  std::istream* input = &std::cin;
  std::ifstream file;

  if (argc > 1)
  {
    file.open(argv[1]);

    if (!file)
    {
      std::cerr << "Error\n";
      return 1;
    }

    input = &file;
  }

  matveev::Stack<long> results;

  std::string line;

  while (std::getline(*input, line))
  {
    if (line.empty())
    {
      continue;
    }

    std::stringstream ss(line);
    std::string token;

    matveev::Queue<std::string> infix;

    while (ss >> token)
    {
      infix.push(token);
    }

    auto postfix = matveev::toPostfix(infix);
    long result = matveev::evaluatePostfix(postfix);

    results.push(result);
  }

  bool first = true;

  while (!results.empty())
  {
    if (!first)
    {
      std::cout << " ";
    }

    std::cout << results.drop();
    first = false;
  }

  std::cout << "\n";

  return 0;
}
