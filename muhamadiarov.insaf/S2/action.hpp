#ifndef ACTION_HPP
#define ACTION_HPP
#include <iosfwd>
#include <string>
#include <list.hpp>
#include <liter.hpp>
#include "queque.hpp"
#include "stack.hpp"

namespace muhamadiarov
{
  using l_d = long long;
  l_d sum(l_d a, l_d b);
  l_d div(l_d a, l_d b);
  l_d sub(l_d a, l_d b);
  l_d mult(l_d a, l_d b);
  l_d mod(l_d a, l_d b);
  l_d xcor(l_d a, l_d b);
  const std::pair< std::string, l_d (*)(l_d, l_d)> oprts[] { {"^", xcor}, {"+", sum}, {"-", sub}, {"*", mult}, {"/", div}, {"%", mod} };
  Queque< Queque < std::string > > input(std::istream& in);
  Queque< std::string > divideString(const std::string str);
  std::istream& getLine(std::istream& in, std::string& str);
  l_d calcExpr(Queque< std::string >& expr);
  bool isPriority(const std::string op1, const std::string op2);
  bool isDigit(const std::string str);
  l_d calc(l_d a, l_d b, const std::string op);
  bool isTrueOprt(const std::string op);
}
#endif
