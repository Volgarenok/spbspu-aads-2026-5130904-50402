#include "math_op.hpp"
#include <climits>
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
  Queue< std::string > res;
  return res;
}
long long karpovich::calculatePostfix(Queue< std::string > postfix)
{
  long long a = 0;
  return a;
}
