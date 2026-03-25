#define ERROR_INVALID_ARGS 1
#define ERROR_OUT_OF_RANGE 2

#include <fstream>
#include <iostream>
#include <istream>
#include <stdexcept>
#include <string>
#include "eval.hpp"
#include "stack.hpp"

namespace shirokov
{
  Stack< std::string > input(std::istream& in);
}

int main(int argc, char** argv)
{
  shirokov::Stack< std::string > expressions;
  if (argc == 1)
  {
    expressions = shirokov::input(std::cin);
  }
  else if (argc == 2)
  {
    std::ifstream in(argv[1]);
    expressions = shirokov::input(in);
  }
  else
  {
    std::cerr << "Incorrect arguments\n";
    return ERROR_INVALID_ARGS;
  }

  while (!expressions.empty())
  {
    long long res;
    try
    {
      res = shirokov::eval(expressions.top());
    }
    catch (const std::invalid_argument&)
    {
      std::cerr << "invalid arg\n";
      return ERROR_INVALID_ARGS;
    }
    catch (const std::out_of_range&)
    {
      std::cerr << "out of range\n";
      return ERROR_OUT_OF_RANGE;
    }
    std::cout << res;
    expressions.pop();
    if (!expressions.empty())
    {
      std::cout << ' ';
    }
  }
  std::cout << '\n';
}

shirokov::Stack< std::string > shirokov::input(std::istream& in)
{
  Stack< std::string > res;
  std::string exp;
  while (std::getline(in, exp))
  {
    res.push(exp);
  }
  return res;
}
