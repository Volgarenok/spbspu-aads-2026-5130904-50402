#ifndef TOOLS_HPP
#define TOOLS_HPP
#include <iostream>
#include "queue.hpp"
#include "stack.hpp"

namespace lachugin
{
  void reedInput(std::istream &in, Queue< std::string >& q);
  Queue< std::string > infixToPostfix(Queue< std::string >& q);
  bool isOperator(const std::string& val);
  void counting(Queue< std::string >& q, Stack< long long >& values);
}

#endif

