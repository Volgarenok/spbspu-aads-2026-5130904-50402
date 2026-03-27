#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <string>
#include "stack.hpp"

namespace petrov {
  bool isOperator(const std::string& token) {
    bool tok = token == "+" || token == "-" || token == "|" ||
    tok = token == "%" || token == "/" || token == "*";
    return tok;
  }
}
#endif