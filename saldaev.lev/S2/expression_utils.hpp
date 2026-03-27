#ifndef EXPRESSION_UTILS_HPP
#define EXPRESSION_UTILS_HPP
#include <cctype>
#include <cstddef>
#include <stdexcept>
#include <string>

namespace saldaev
{
  inline bool isNumber(std::string token)
  {
    for (size_t i = 0; i < token.size(); ++i) {
      if (!std::isdigit(token[i])) {
        return false;
      }
    }
    return true;
  }

  inline bool isOperator(std::string token)
  {
    if (token == "+" || token == "-" || token == "*" || token == "/" || token == "%" || token == "!") {
      return true;
    }
    return false;
  }

  inline int getPriority(const std::string &op)
  {
    if (op == "+" || op == "-") {
      return 1;
    }
    if (op == "*" || op == "/" || op == "%") {
      return 2;
    }
    return 0;
  }

  inline long long apply(const std::string &op, long long left, long long right)
  {
    if (op == "+") {
      return left + right;
    } else if (op == "-") {
      return left - right;
    } else if (op == "*") {
      return left * right;
    } else if (op == "/") {
      return left / right;
    } else if (op == "%") {
      return left % right;
    } else {
      throw std::logic_error("invalid operator");
    }
  }

  inline long long apply(const std::string &op, long long rihgt)
  {
    if (op == "!") {
      return ~rihgt;
    } else {
      throw std::logic_error("invalid operator");
    }
  }
}

#endif
