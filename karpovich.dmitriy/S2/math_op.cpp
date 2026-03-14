#include "math_op.hpp"
#include "stack.hpp"
#include <climits>
#include <cstddef>
#include <stdexcept>

long long karpovich::sum(long long a, long long b)
{
  if ((b > 0 && a > LLONG_MAX - b) || (b < 0 && a < LLONG_MIN - b)) {
    throw std::runtime_error("Overflow");
  }
  return a + b;
}
long long karpovich::divide(long long a, long long b)
{
  if (b == 0) {
    throw std::runtime_error("Division by zero");
  }
  return a / b;
}
long long karpovich::subtract(long long a, long long b)
{
  if ((b > 0 && a < LLONG_MIN + b) || (b < 0 && a > LLONG_MAX + b)) {
    throw std::runtime_error("Overflow");
  }
  return a - b;
}
long long karpovich::mod(long long a, long long b)
{
  if (b == 0) {
    throw std::runtime_error("Division by zero");
  }
  return a % b;
}
long long karpovich::multiply(long long a, long long b)
{
  if (a > 0) {
    if (b > 0 && a > LLONG_MAX / b) {
      throw std::runtime_error("Overflow");
    }
    if (b < 0 && b < LLONG_MIN / a) {
      throw std::runtime_error("Overflow");
    }
  } else {
    if (b > 0 && a < LLONG_MIN / b) {
      throw std::runtime_error("Overflow");
    }
    if (b < 0 && a < LLONG_MAX / b) {
      throw std::runtime_error("Overflow");
    }
  }
  return a * b;
}
karpovich::Queue< std::string > karpovich::convertToPostfix(Queue< std::string > infix)
{
  Queue< std::string > postfix;
  Stack< std::string > tempStack;
  while (!infix.empty()) {
    std::string current = infix.front();
    infix.pop();
    if (current == "(") {
      tempStack.push(current);
      continue;
    }
    if (isOperation(current)) {
      while (isPriorityOp(current, tempStack.top()) && !tempStack.empty() && tempStack.top() != "(") {
        postfix.push(tempStack.top());
        tempStack.pop();
      }
      tempStack.push(current);
      continue;
    }
    if (current == ")") {
      while (!tempStack.empty() && tempStack.top() != "(") {
        postfix.push(tempStack.top());
        tempStack.pop();
      }
      if (!tempStack.empty()) {
        tempStack.pop();
      }
      continue;
    }
    postfix.push(current);
  }
  while (!tempStack.empty()) {
    postfix.push(tempStack.top());
    tempStack.pop();
  }
  return postfix;
}
bool karpovich::isOperation(const std::string &op)
{
  return op == "+" || op == "-" || op == "/" || op == "*" || op == "^" || op == "%";
}
bool karpovich::isPriorityOp(const std::string &op1, const std::string &op2)
{
  size_t prior1 = 0;
  size_t prior2 = 0;
  if (op1 == "+" || op1 == "-") {
    prior1 = 1;
  } else {
    prior1 = 2;
  }
  if (op2 == "+" || op2 == "-") {
    prior2 = 1;
  } else {
    prior2 = 2;
  }
  return prior1 >= prior2;
}
long long karpovich::calculatePostfix(Queue< std::string > postfix)
{
  long long a = 0;
  postfix.pop();
  return a;
}
