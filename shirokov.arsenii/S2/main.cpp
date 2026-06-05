#include <exception>
#include <fstream>
#include <iostream>
#include <istream>
#include <new>
#include <string>
#include "eval.hpp"
#include "stack.hpp"

namespace shirokov
{
  Stack< std::string > input(std::istream& in);
}

int main(int argc, char** argv)
{
  if (argc > 2)
  {
    std::cerr << "Incorrect arguments\n";
    return 1;
  }

  try
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
      in.close();
    }

    while (!expressions.empty())
    {
      long long res;
      res = shirokov::eval(expressions.top());
      std::cout << res;
      expressions.pop();
      if (!expressions.empty())
      {
        std::cout << ' ';
      }
    }
    std::cout << '\n';
  }
  catch (const std::bad_alloc&)
  {
    std::cerr << "Memory allocation error when adding an element\n";
    return 1;
  }
  catch (const std::exception& e)
  {
    std::cerr << "Calculate error: " << e.what() << '\n';
    return 1;
  }
}

shirokov::Stack< std::string > shirokov::input(std::istream& in)
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
