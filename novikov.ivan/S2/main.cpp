#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept>
#include "stack.hpp"
#include "eval.hpp"

namespace novikov
{
  Stack< std::string > input(std::istream& in);
}

int main(int argc, char** argv)
{
  novikov::Stack< std::string > expressions;
  if (argc == 1)
  {
    try
    {
      expressions = novikov::input(std::cin);
    }
    catch (const std::bad_alloc&)
    {
      std::cerr << "Bad allocation\n";
      return 2;
    }
  }
  else if (argc == 2)
  {
    try
    {
      std::ifstream in(argv[1]);
      expressions = novikov::input(in);
      in.close();
    }
    catch (const std::bad_alloc&)
    {
      std::cerr << "Bad allocation\n";
      return 2;
    }
  }
  else
  {
    std::cerr << "Invalid arguments\n";
    return 1;
  }

  while (!expressions.empty())
  {
    long long res;
    try
    {
      res = novikov::eval(expressions.top());
    }
    catch (...)
    {
      std::cerr << "Unknown error\n";
      return 3;
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

novikov::Stack< std::string > novikov::input(std::istream& in)
{
  Stack< std::string > res;
  std::string exp;
  while (std::getline(in, exp))
  {
    if (!exp.empty())
    {
      res.push(exp);
    }
  }
  return res;
}
