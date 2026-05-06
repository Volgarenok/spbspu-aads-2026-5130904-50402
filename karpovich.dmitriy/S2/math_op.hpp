#ifndef MATH_OP_HPP
#define MATH_OP_HPP
#include <string>
#include "queue.hpp"

namespace karpovich
{
  long long sum(long long a, long long b);
  long long divide(long long a, long long b);
  long long subtract(long long a, long long b);
  long long mod(long long a, long long b);
  long long multiply(long long a, long long b);
  Queue< std::string > convertToPostfix(Queue< std::string > infix);
  long long calculatePostfix(Queue< std::string > postfix);
  bool isOperation(const std::string &op);
  bool isPriorityOp(const std::string &op1, const std::string &op2);
  long long calculateOperation(const std::string &op, long long a, long long b);
}
#endif
