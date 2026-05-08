#include "parserUtils.hpp"

#include <iostream>
#include <string>

int main()
{
  matveev::List< std::string > args;

  args.insertAfter(args.beforeBegin(), "b");
  args.insertAfter(args.beforeBegin(), "a");

  std::cout << matveev::countArgs(args) << '\n';
  std::cout << matveev::hasArgCount(args, 2) << '\n';
  std::cout << matveev::hasArgCount(args, 3) << '\n';

  return 0;
}
