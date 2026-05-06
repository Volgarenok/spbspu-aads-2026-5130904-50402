#include "math_op.hpp"
#include "stack.hpp"
#include <climits>
#include <cstddef>
#include <stdexcept>
#include <string>

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
  return (a % b + b) % b;
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
      while (!tempStack.empty() && isPriorityOp(current, tempStack.top()) && tempStack.top() != "(") {
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
long long karpovich::calculateOperation(const std::string &op, long long a, long long b)
{
  if (op == "+") {
    return sum(a, b);
  } else if (op == "-") {
    return subtract(a, b);
  } else if (op == "/") {
    return divide(a, b);
  } else if (op == "%") {
    return mod(a, b);
  } else if (op == "*") {
    return multiply(a, b);
  } else if (op == "^") {
    return a ^ b;
  } else {
    throw std::runtime_error("bad operation");
  }
}
long long karpovich::calculatePostfix(Queue< std::string > postfix)
{
  Stack< long long > tempStack;
  while (!postfix.empty()) {
    std::string current = postfix.front();
    postfix.pop();
    if (isOperation(current)) {
      if (tempStack.empty()) {
        throw std::runtime_error("Invalid expression");
      }
      long long b = tempStack.top();
      tempStack.pop();
      if (tempStack.empty()) {
        throw std::runtime_error("Invalid expression");
      }
      long long a = tempStack.top();
      tempStack.pop();
      tempStack.push(calculateOperation(current, a, b));
    } else {
      tempStack.push(std::stoll(current));
    }
  }
  if (tempStack.empty()) {
    throw std::runtime_error("Empty result");
  }
  return tempStack.top();
}
