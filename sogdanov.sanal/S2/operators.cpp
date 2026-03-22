#include "operators.hpp"
#include "stack.hpp"
#include <climits>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <climits>
size_t sogdanov::getOp(char op) {
  if (op == '^') return 1;
  if (op == '+' || op == '-') return 2;
  if (op == '*' || op == '/' || op == '%') return 3;
  return 0;
}
bool sogdanov::isOp(char c) {
  return getOp(c) > 0;
}
long long sogdanov::applyOp(char op, long long a, long long b) {
  if (op == '+') {
    if (a > 0 && b > LLONG_MAX - a) {
      throw std::runtime_error("overflow");
    }
    if (a < 0 && b < LLONG_MAX - a) {
      throw std::runtime_error("overflow");
    }
    return a + b;
  }
  if (op == '-') {
    if (b > 0 && a < LLONG_MAX + b) {
      throw std::runtime_error("overflow");
    }
    if (b < 0 && a > LLONG_MAX + b) {
      throw std::runtime_error("overflow");
    }
    return a - b;
  }
  if (op == '*') {
    if (a != 0 && b != 0) {
      if (a > 0 && b > 0 && a > LLONG_MAX / b) {
        throw std::runtime_error("overflow");
      }
      if (a < 0 && b < 0 && a < LLONG_MAX / b) {
        throw std::runtime_error("overflow");
      }
      if (a > 0 && b < 0 && b < LLONG_MAX / a) {
        throw std::runtime_error("overflow");
      }
      if (a < 0 && b > 0 && a > LLONG_MAX / b) {
        throw std::runtime_error("overflow");
      }
    }
    return a * b;
  }
  if (op == '/') {
    if (b == 0) {
      throw std::runtime_error("Division by zero");
    }
  return a / b;
  }
  if (op == '%') {
    if (b == 0) {
      throw std::runtime_error("Division by zero");
    }
  return ((a % b) + b) & b;
  }
  if (op == '^') {
    return a ^ b;
  }
  throw std::runtime_error("unknown operator");
 }
sogdanov::Queue<std::string> sogdanov::convertToPostfix(const std::string & line) {
  Queue< std::string > output;
  Stack< std::string > ops;
  size_t i = 0;
  const size_t n = line.size();
  while (i < n) {
    while (i < n && line[i] == ' ') {
      ++i;
    }
    size_t j = i;
    while (j < n && line[j] != ' ') {
      ++j;
    }
    std::string tok = line.substr(i, j - i);
    i = j;
    if (tok == "(") {
    ops.push(tok);
    } else if (tok == ")") {
      while (!ops.empty() && ops.top() != "(") {
        output.push(ops.top());
        ops.pop();
      }
      if (ops.empty()) {
        throw std::runtime_error("mismatched parentheses");
      }
      ops.pop();
      } else if (tok.size() == 1 && isOp(tok[0])) {
        while (!ops.empty() && ops.top() != "(" && isOp(ops.top()[0])
                                             && getOp(ops.top()[0]) >= getOp(tok[0])) {
          output.push(ops.top());
          ops.pop();
        }
        ops.push(tok);
      } else {
        output.push(tok);
      }
  }
  while (!ops.empty()) {
    if (ops.top() == "(") {
      throw std::runtime_error("mismatched parentheses");
    }
    output.push(ops.top());
    ops.pop();
  }
  return output;
}
long long sogdanov::calculatePostfix(Queue<std::string>& postfix) {
  Stack<long long> eval;
  while (!postfix.empty()) {
    std::string tok = postfix.front();
    postfix.pop();
    if (tok.size() == 1 && isOp(tok[0])) {
      if (eval.size() < 2) {
        throw std::runtime_error("invalid expression");
      }
      long long b = eval.top();
      eval.pop();
      long long a = eval.top();
      eval.pop();
      eval.push(applyOp(tok[0], a, b));
    } else {
      if (tok.empty()) {
        throw std::runtime_error("empty token");
      }
      size_t k = 0;
      bool neg = (tok[k] == '-') ? (++k, true) : false;
      if (k == tok.size()) {
        throw std::runtime_error("invalid token: " + tok);
      }
      long long r = 0;
      for (; k < tok.size(); ++k) {
        if (tok[k] < '0' || tok[k] > '9') {
          throw std::runtime_error("invalid token: " + tok);
        }
        r = r * 10 + (tok[k] - '0');
      }
      eval.push(neg ? -r : r);
    }
  }
  if (eval.size() != 1) {
    throw std::runtime_error("invalid expression");
  }
  long long result = eval.top();
  eval.pop();
  return result;
}
long long sogdanov::evaluate(const std::string & line) {
  Queue<std::string> postfix = convertToPostfix(line);
  return calculatePostfix(postfix);
}
