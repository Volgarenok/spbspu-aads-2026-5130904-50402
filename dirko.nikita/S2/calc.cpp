#include "calc.hpp"
#include <cstddef>
#include <limits>
#include <stdexcept>
#include <string>
#include "queue.hpp"
#include "stack.hpp"

bool dirko::getPriority(const std::string &op1, const std::string &op2)
{
  short pr1 = 0, pr2 = 0;
  if (op1 == "lcm") {
    pr1 = 3;
  } else if (op1 == "+" || op1 == "-") {
    pr1 = 1;
  } else {
    pr1 = 2;
  }
  if (op2 == "lcm") {
    pr2 = 3;
  } else if (op2 == "+" || op2 == "-") {
    pr2 = 1;
  } else {
    pr2 = 2;
  }
  return pr1 >= pr2;
}

bool dirko::isOpt(std::string candidate)
{
  bool ret = false;
  for (size_t i = 0; i < dirko::opt_size; ++i) {
    ret = ret || (candidate == opts[i]);
  }
  return ret;
}

dirko::Queue< std::string > dirko::convert(dirko::Queue< std::string > inf)
{
  Queue< std::string > ret;
  Stack< std::string > st;
  while (!inf.empty()) {
    std::string curr = inf.get();
    inf.pop();
    if (curr == "(") {
      st.push(curr);
      continue;
    }
    if (isOpt(curr)) {
      while (!st.empty() && getPriority(curr, st.get()) && st.get() != "(") {
        ret.push(st.get());
        st.pop();
      }
      st.push(curr);
      continue;
    }
    if (curr == ")") {
      while (!st.empty() && st.get() != "(") {
        ret.push(st.get());
        st.pop();
      }
      if (!st.empty()) {
        st.pop();
      }
      continue;
    }
    ret.push(curr);
  }
  while (!st.empty()) {
    ret.push(st.get());
    st.pop();
  }
  return ret;
}

long long dirko::calc(const std::string &opt, long long a, long long b)
{
  if (opt == "+") {
    if (std::numeric_limits< long long >::max() - a < b) {
      throw std::overflow_error("overflow");
    }
    return a + b;
  } else if (opt == "-") {
    if (std::numeric_limits< long long >::min() + a > b) {
      throw std::underflow_error("underflow");
    }
    return a - b;
  } else if (opt == "*") {
    if (a > 0) {
      if (b > 0 && a > std::numeric_limits< long long >::max() / b) {
        throw std::overflow_error("Overflow");
      }
      if (b < 0 && b < std::numeric_limits< long long >::min() / a) {
        throw std::overflow_error("Overflow");
      }
    } else {
      if (b > 0 && a < std::numeric_limits< long long >::min() / b) {
        throw std::overflow_error("Overflow");
      }
      if (b < 0 && a < std::numeric_limits< long long >::max() / b) {
        throw std::overflow_error("Overflow");
      }
    }
    return a * b;
  } else if (opt == "/") {
    if (b == 0) {
      throw std::logic_error("zero dividion");
    }
    return a / b;
  } else if (opt == "%") {
    if (b == 0) {
      throw std::logic_error("zero dividion");
    }
    return (a % b + b) % b;
  } else if (opt == "lcm") {
    return lcm(a, b);
  } else {
    throw std::runtime_error("unknown opt");
  }
}

long long dirko::calcExpr(Queue< std::string > expretion)
{
  Stack< long long > st;
  while (!expretion.empty()) {
    std::string curr = expretion.get();
    expretion.pop();
    if (isOpt(curr)) {
      if (st.empty()) {
        throw std::runtime_error("invalid expretion");
      }
      long long b = st.get();
      st.pop();
      if (st.empty()) {
        throw std::runtime_error("invalid expretion");
      }
      long long a = st.get();
      st.pop();
      st.push(calc(curr, a, b));
    } else {
      st.push(std::stoll(curr));
    }
  }
  if (st.empty()) {
    throw std::runtime_error("empty result");
  }
  return st.get();
}

long long dirko::lcm(long long a, long long b)
{

  long long res = std::min(a, b);
  while (res > 1) {
    if (a % res == 0 && b % res == 0)
      break;
    res--;
  }
  return a * b / res;
}
