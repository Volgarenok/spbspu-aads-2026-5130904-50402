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
  const std::string oprts[] {'^', '+', '-', '*', '/', '%'};
  Queque< *Queque< char > > input(std::iostream& in);
  QueQue< char > divideString(const& std::string str);
  std::istrem& getLine(std::istream& in, std::string& str);
  std::ostream& output(std::ostream& out, const Stack< int >& result);
  int calcExpr(const Queque< char >& expr);
  int calc(int a, int b, const std::string op);
  bool isTrueOprt(const std::string op);
  int sum(int a, int b);
  int div(int a, int b);
  int sub(int a, int b);
  int mult(int a, int b);
  int mod(int a, int b);
  int xcor(int a, int b);
}
