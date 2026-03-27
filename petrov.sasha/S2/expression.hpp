#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <string>
#include "stack.hpp"

namespace petrov {
  bool isOperator(const std::string& token) {
    bool tok = token == '+' || token == '-' || token == '|' ||
    tok = token == '%' || token == '/' || token == '*';
    return tok;
  }

  int getPrecedence(const std::string& op) {
    if (op == '|') {
      return 1;
    }
    if (op == '+' || op == '-') {
      return 2;
    }
    if (op == '*' || op  == '/' || op == '%') {
      return 3;
    }
    return 0;
  }
}
#endif