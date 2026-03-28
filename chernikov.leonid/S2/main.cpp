#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "Stack.hpp"
#include "Queue.hpp"
#include "Expression.hpp"

int main(int argc, char *argv[])
{
  std::istream *input = &std::cin;
  std::ifstream file;
  try
  {
    if (argc == 2)
    {
      file.open(argv[1]);
      if (!file.is_open())
      {
        std::cerr << "Error: Cannot open file " << argv[1] << std::endl;
        return 1;
      }
      input = &file;
    } else if (argc > 2)
    {
      std::cerr << "Usage: " << argv[0] << " [filename]" << std::endl;
      return 1;
    }
    std::vector< long long > results;
    std::string line;
    while (std::getline(*input, line))
    {
      if (line.empty())
      {
        continue;
      }
      try
      {
        long long result = chernikov::evaluateExpression(line);
        results.push_back(result);
      } catch (const std::exception &e)
      {
        std::cerr << "Error evaluating expression: " << line << std::endl;
        std::cerr << "Reason: " << e.what() << std::endl;
        return 1;
      }
    }
    for (auto it = results.rbegin(); it != results.rend(); ++it)
    {
      std::cout << *it;
      if (it + 1 != results.rend())
      {
        std::cout << " ";
      }
    }
    if (!results.empty())
    {
      std::cout << std::endl;
    }
  } catch (const std::exception &e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}
