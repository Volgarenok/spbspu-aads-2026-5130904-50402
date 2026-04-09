#ifndef OPERATIONS_HPP
#define OPERATIONS_HPP
#include <iostream>
namespace lachugin
{
  int sum(int& a, int& b);
  int subtraction(int& a, int& b);
  int multiply(int& a, int& b);
  int divide(int& a, int& b);
  int mod(int& a, int& b);
  bool isPriority(const std::string& o1, const std::string& o2);
}
#endif
