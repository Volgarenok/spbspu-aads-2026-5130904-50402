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
  int sum(int a, int b);
  int div(int a, int b);
  int sub(int a, int b);
  int mult(int a, int b);
  int mod(int a, int b);
  int xcor(int a, int b);
  const std::pair< std::string, int (*)(int, int)> oprts[] { {"^", xcor}, {"+", sum}, {"-", sub}, {"*", mult}, {"/", div}, {"%", mod} };
  Queque< Queque< std::string* > > input(std::iostream& in);
  Queque< std::string > divideString(const std::string str);
  std::istream& getLine(std::istream& in, std::string& str);
  std::ostream& output(std::ostream& out, const Stack< int >& result);
  int calcExpr(Queque< std::string >& expr);
  bool isPriority(const std::string op1, const std::string op2);
  bool isDigit(const std::string str);
  int calc(int a, int b, const std::string op);
  bool isTrueOprt(const std::string op);
}
#endif
