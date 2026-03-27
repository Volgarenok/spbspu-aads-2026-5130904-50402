#ifndef NOVIKOV_EVAL_HPP
#define NOVIKOV_EVAL_HPP

#include "stack.hpp"
#include "queue.hpp"
#include <string>
#include <sstream>
#include <cctype>
#include <stdexcept>

namespace novikov
{
  int priority(const std::string &op)
  {
    if (op == "+" || op == "-")
      return 1;
    if (op == "*" || op == "/" || op == "%")
      return 2;
    if (op == "^")
      return 0;
    return 0;
  }

  bool isNumber(const std::string &s)
  {
    if (s.empty())
      return false;
    if (s[0] == '-' && s.size() > 1)
      return true;
    return std::isdigit(s[0]);
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
        if (ops.empty()) {
          std::cerr << "Bracket mismatch\n";
          throw;
        }
        ops.drop();
      } else {
        while (!ops.empty() && priority(ops.front()) >= priority(token)) {
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

  int apply(int a, int b, const std::string &op)
  {
    if (op == "+")
      return a + b;
    if (op == "-")
      return a - b;
    if (op == "*")
      return a * b;
    if (op == "/") {
      if (b == 0) {
        std::cerr << "Division by zero\n";
        throw;
      }
      return a / b;
    }
    if (op == "%") {
      if (b == 0) {
        std::cerr << "Division by zero\n";
        throw;
      }
      return a % b;
    }
    if (op == "^") {
      return a ^ b;
    }
    std::cerr << "Unknown operator: " << op << "\n";
    throw;
  }

  int eval(const std::string &expr)
  {
    Queue< std::string > postfix = toPostfix(expr);
    Stack< int > st;

    while (!postfix.empty()) {
      std::string token = postfix.drop();
      if (isNumber(token)) {
        st.push(std::stoi(token));
      } else {
        int b = st.drop();
        int a = st.drop();
        st.push(apply(a, b, token));
      }
    }
    return st.drop();
  }
}
#endif
