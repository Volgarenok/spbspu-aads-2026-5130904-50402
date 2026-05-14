#ifndef NOVIKOV_EVAL_HPP
#define NOVIKOV_EVAL_HPP

#include "stack.hpp"
#include "queue.hpp"
#include <string>
#include <sstream>
#include <cctype>
#include <stdexcept>
#include <limits>

namespace novikov
{
  int priority(const std::string &op)
  {
    if (op == "+" || op == "-")
      return 1;
    if (op == "*" || op == "/" || op == "%")
      return 2;
    return 0;
  }

  bool isNumber(const std::string &s)
  {
    if (s.empty())
      return false;
    size_t start = (s[0] == '-') ? 1 : 0;
    for (size_t i = start; i < s.size(); ++i) {
      if (!std::isdigit(s[i]))
        return false;
    }
    return true;
  }

  long long safe_stoll(const std::string &s)
  {
    std::istringstream iss(s);
    long long result;
    if (iss >> result)
      return result;
    throw std::runtime_error("Invalid number: " + s);
  }

  Queue< std::string > toPostfix(const std::string &expr)
  {
    Stack< std::string > ops;
    Queue< std::string > output;
    std::istringstream iss(expr);
    std::string token;

    while (iss >> token) {
      if (isNumber(token)) {
        output.push(token);
      } else if (token == "(") {
        ops.push(token);
      } else if (token == ")") {
        while (!ops.empty() && ops.front() != "(") {
          output.push(ops.drop());
        }
        if (ops.empty())
          throw std::runtime_error("Bracket mismatch");
        ops.drop();
      } else {
        while (!ops.empty() && ops.front() != "(" && priority(ops.front()) >= priority(token)) {
          output.push(ops.drop());
        }
        ops.push(token);
      }
    }

    while (!ops.empty()) {
      output.push(ops.drop());
    }
    return output;
  }

  long long apply(long long a, long long b, const std::string &op)
  {
    if (op == "+") {
      if ((b > 0 && a > std::numeric_limits< long long >::max() - b)
          || (b < 0 && a < std::numeric_limits< long long >::min() - b)) {
        throw std::runtime_error("Overflow");
      }
      return a + b;
    }
    if (op == "-") {
      if ((b < 0 && a > std::numeric_limits< long long >::max() + b)
          || (b > 0 && a < std::numeric_limits< long long >::min() + b)) {
        throw std::runtime_error("Overflow");
      }
      return a - b;
    }
    if (op == "*") {
      if (a != 0 && b != 0) {
        if ((a > 0 && b > 0 && a > std::numeric_limits< long long >::max() / b)
            || (a > 0 && b < 0 && b < std::numeric_limits< long long >::min() / a)
            || (a < 0 && b > 0 && a < std::numeric_limits< long long >::min() / b)
            || (a < 0 && b < 0 && a < std::numeric_limits< long long >::max() / b)) {
          throw std::runtime_error("Overflow");
        }
      }
      return a * b;
    }
    if (op == "/") {
      if (b == 0)
        throw std::runtime_error("Division by zero");
      if (a == std::numeric_limits< long long >::min() && b == -1)
        throw std::runtime_error("Overflow");
      return a / b;
    }
    if (op == "%") {
      if (b == 0)
        throw std::runtime_error("Division by zero");
      long long result = a % b;
      if (result < 0)
        result += (b < 0 ? -b : b);
      return result;
    }
    if (op == "^")
      return a ^ b;
    throw std::runtime_error("Unknown operator: " + op);
  }

  long long eval(const std::string &expr)
  {
    Queue< std::string > postfix = toPostfix(expr);
    Stack< long long > st;

    while (!postfix.empty()) {
      std::string token = postfix.drop();
      if (isNumber(token)) {
        st.push(safe_stoll(token));
      } else {
        long long b = st.drop();
        long long a = st.drop();
        st.push(apply(a, b, token));
      }
    }
    return st.drop();
  }
}
#endif
