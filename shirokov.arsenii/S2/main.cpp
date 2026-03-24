#include <fstream>
#include <iostream>
#include <istream>
#include <string>
#include "stack.hpp"

namespace shirokov
{
  Stack< std::string > input(std::istream& in);
  int eval(std::string line);
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
    return 1;
  }

  while (!expressions.empty())
  {
    int res = shirokov::eval(expressions.top());
    std::cout << res;
    expressions.pop();
    if (!expressions.empty())
    {
      std::cout << ' ';
    }
  }
  std::cout << '\n';
}
