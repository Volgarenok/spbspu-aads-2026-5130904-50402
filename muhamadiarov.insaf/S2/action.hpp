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
  long long sum(long long a, long long b);
  long long div(long long a, long long b);
  long long sub(long long a, long long b);
  long long mult(long long a, long long b);
  long long mod(long long a, long long b);
  long long xcor(long long a, long long b);
  const std::pair< std::string, long long (*)(long long, long long)> oprts[] { {"^", xcor}, {"+", sum}, {"-", sub}, {"*", mult}, {"/", div}, {"%", mod} };
  Queque< Queque < std::string > > input(std::istream& in);
  Queque< std::string > divideString(const std::string str);
  std::istream& getLine(std::istream& in, std::string& str);
  long long calcExpr(Queque< std::string >& expr);
  bool isPriority(const std::string op1, const std::string op2);
  bool isDigit(const std::string str);
  long long calc(long long a, long long b, const std::string op);
  bool isTrueOprt(const std::string op);
}
#endif
