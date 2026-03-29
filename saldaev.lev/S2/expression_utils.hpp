#ifndef EXPRESSION_UTILS_HPP
#define EXPRESSION_UTILS_HPP
#include <cctype>
#include <cstddef>
#include <limits>
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
      return sum(left, right);
    } else if (op == "-") {
      return substract(left, right);
    } else if (op == "*") {
      return multiply(left, right);
    } else if (op == "/") {
      return division(left, right);
    } else if (op == "%") {
      return mod(left, right);
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

  long long parseNumber(const std::string &token)
  {
    long long result = 0;
    for (char c : token) {
      result = result * 10 + (c - '0');
    }
    return result;
  }
  long long sum(long long left, long long right)
  {
    if (left >= 0) {
      if (std::numeric_limits< long long >::max() - left < right) {
        throw std::overflow_error("overflow");
      }
    } else {
      if (std::numeric_limits< long long >::min() - left > right) {
        throw std::overflow_error("underflow");
      }
    }
    return left + right;
  }

  long long substract(long long left, long long right)
  {
    if (left >= 0) {
      if (std::numeric_limits< long long >::max() - left < right) {
        throw std::overflow_error("overflow");
      }
    } else {
      if (std::numeric_limits< long long >::min() + left > right) {
        throw std::overflow_error("underflow");
      }
    }
    return left - right;
  }

  long long multiply(long long left, long long right)
  {
    if (left == 0 || right == 0) {
      return 0;
    }

    if (left == std::numeric_limits< long long >::min()) {
      if (right == 1)
        return std::numeric_limits< long long >::min();
      throw std::overflow_error("overflow");
    }
    if (right == std::numeric_limits< long long >::min()) {
      if (left == 1)
        return std::numeric_limits< long long >::min();
      throw std::overflow_error("overflow");
    }

    if (left > 0) {
      if (right > 0) {
        if (right > std::numeric_limits< long long >::max() / left) {
          throw std::overflow_error("overflow");
        }
      } else {
        if (right < std::numeric_limits< long long >::min() / left) {
          throw std::overflow_error("underflow");
        }
      }
    } else {
      if (right > 0) {
        if (right > std::numeric_limits< long long >::min() / left) {
          throw std::overflow_error("underflow");
        }
      } else {
        if (right < std::numeric_limits< long long >::max() / left) {
          throw std::overflow_error("overflow");
        }
      }
    }

    return left * right;
  }

  long long division(long long left, long long right)
  {
    if (right == 0) {
      throw std::logic_error("division by zero");
    }

    if (left == std::numeric_limits< long long >::min() && right == -1) {
      throw std::overflow_error("overflow");
    }

    return left / right;
  }

  long long mod(long long left, long long right)
  {
    if (right == 0) {
      throw std::logic_error("modulo by zero");
    }

    if (left == std::numeric_limits< long long >::min() && right == -1) {
      throw std::overflow_error("overflow");
    }

    return left % right;
  }

}

#endif
