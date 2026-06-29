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
  namespace
  {
    const long long LMAX = std::numeric_limits< long long >::max();
    const long long LMIN = std::numeric_limits< long long >::min();
    int priority(const std::string &op)
    {
      if (op == "+" || op == "-")
        return 1;
      if (op == "*" || op == "/" || op == "%")
        return 2;
      if (op == "^")
        return 3;
      return 0;
    }

    long long safe_stoll(const std::string &s)
    {
      size_t pos;
      long long result = std::stoll(s, &pos);
      if (pos != s.size()) {
        throw std::runtime_error("Invalid number: " + s);
      }
      return result;
    }

    long long apply(long long a, long long b, const std::string &op)
    {
      if (op == "+") {
        if ((b > 0 && a > LMAX - b) || (b < 0 && a < LMIN - b)) {
          throw std::runtime_error("Overflow");
        }
        return a + b;
      }
      if (op == "-") {
        if ((b < 0 && a > LMAX + b) || (b > 0 && a < LMIN + b)) {
          throw std::runtime_error("Overflow");
        }
        return a - b;
      }
      if (op == "*") {
        if (a != 0 && b != 0) {
          if ((a > 0 && b > 0 && a > LMAX / b) || (a > 0 && b < 0 && b < LMIN / a)
              || (a < 0 && b > 0 && a < LMIN / b) || (a < 0 && b < 0 && a < LMAX / b)) {
            throw std::runtime_error("Overflow");
          }
        }
        return a * b;
      }
      if (op == "/") {
        if (b == 0)
          throw std::runtime_error("Division by zero");
        if (a == LMIN && b == -1)
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
  }

  Queue< std::string > toPostfix(const std::string &expr)
  {
    Stack< std::string > ops;
    Queue< std::string > output;
    std::istringstream iss(expr);
    std::string token;

    while (iss >> token) {
      if (token == "(") {
        ops.push(token);
      } else if (token == ")") {
        while (!ops.empty() && ops.front() != "(") {
          output.push(ops.drop());
        }
        if (ops.empty()) {
          throw std::runtime_error("Bracket mismatch");
        }
        ops.drop();
      } else if (token == "+" || token == "-" || token == "*" || token == "/" || token == "%" || token == "^") {
        while (!ops.empty() && ops.front() != "(" && priority(ops.front()) >= priority(token)) {
          output.push(ops.drop());
        }
        ops.push(token);
      } else {
        output.push(token);
      }
    }

    while (!ops.empty()) {
      output.push(ops.drop());
    }
    return output;
  }

  long long eval(const std::string &expr)
  {
    Queue< std::string > postfix = toPostfix(expr);
    Stack< long long > st;
    while (!postfix.empty()) {
      std::string token = postfix.drop();
      if (token == "+" || token == "-" || token == "*" || token == "/" || token == "%" || token == "^") {
        long long b = st.drop();
        long long a = st.drop();
        st.push(apply(a, b, token));
      } else {
        st.push(safe_stoll(token));
      }
    }
    return st.drop();
  }
}
#endif
