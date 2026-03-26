#include "funcs.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <cctype>
#include "datastructs.hpp"

int strelnikov::getPriority(const std::string &a)
{
  if (a == "+" || a == "-") {
    return 1;
  }
  if (a == "/" || a == "*" || a == "%") {
    return 2;
  }
  if (a == "!") {
    return 3;
  }

	return 0;
}

bool strelnikov::checkPriority(const std::string &a, const std::string &b)
{
  return getPriority(a) >= getPriority(b);
}

bool strelnikov::isOp(const std::string &a)
{
  return a == "+" || a == "-" || a == "*" || a == "%" || a == "/" || a == "!";
}

bool strelnikov::isNumber(const std::string &token)
{
  if (token.empty()) {
    return false;
  }
  size_t pos = 0;
  if (token[0] == '-') {
    if (token.length() == 1) {
      return false;
    }
    pos = 1;
  }

  for (size_t i = pos; i < token.length(); ++i) {
    if (!std::isdigit(static_cast< unsigned char >(token[i]))) {
      return false;
    }
  }
  return true;
}

long long strelnikov::calcOps(const std::string &op, long long a, long long b)
{
  if (op == "+") {
    return a + b;
  }
  if (op == "-") {
    return a - b;
  }
  if (op == "*") {
    return a * b;
  }
  if (op == "/") {
    return a / b;
  }
  if (op == "%") {
    return a % b;
  }
  return 0;
}

strelnikov::Queue< strelnikov::Queue< std::string > > strelnikov::input(std::istream &in)
{
  Queue< Queue< std::string > > expr;
  std::string line;

  while (std::getline(in, line)) {
    if (line.empty()) {
      continue;
    }

    bool hasContent = false;
    for (char c : line) {
      if (!std::isspace(static_cast< unsigned char >(c))) {
        hasContent = true;
        break;
      }
    }
    if (!hasContent) {
      continue;
    }

    Queue< std::string > tokens;
    std::istringstream iss(line);
    std::string token;

    while (iss >> token) {
      tokens.push(token);
    }

    expr.push(tokens);
  }

  return expr;
}

long long strelnikov::calc(Queue< std::string > expr)
{
  Stack< long long > stack;

  while (!expr.empty()) {
    std::string token = expr.get();
    expr.pop();

    if (isNumber(token)) {
      long long num = std::stoll(token);
      stack.push(num);
    } else if (isOp(token)) {
      if (stack.empty()) {
        throw std::runtime_error("Invalid expression: not enough operands");
      }
      long long result = 0;
      if (token == "!") {
        long long a = stack.get();
        stack.pop();
        result = ~a;
      } else {
        long long b = stack.get();
        stack.pop();

        if (stack.empty()) {
          throw std::runtime_error("Invalid expression: not enough operands");
        }
        long long a = stack.get();
        stack.pop();

        if ((token == "/" || token == "%") && b == 0) {
          throw std::runtime_error("Division by zero");
        }

        result = calcOps(token, a, b);
      }

      stack.push(result);
    } else {
      throw std::runtime_error("Invalid token: " + token);
    }
  }

  if (stack.empty()) {
    throw std::runtime_error("Invalid expression: empty result");
  }

  return stack.get();
}

strelnikov::Queue< std::string > strelnikov::convertToPostfix(Queue< std::string > curr)
{
  Queue< std::string > res;
  Stack< std::string > buff;
  while (!curr.empty()) {
    std::string token = curr.get();
    curr.pop();
    if (token == "(") {
      buff.push(token);
    } else if (isOp(token)) {
      while (!buff.empty() && buff.get() != "(" && checkPriority(buff.get(), token)) {
        res.push(buff.get());
        buff.pop();
      }
      buff.push(token);
    } else if (token == ")") {
      while (!buff.empty() && buff.get() != "(") {
        res.push(buff.get());
        buff.pop();
      }
      if (!buff.empty()) {
        buff.pop();
      }
    } else {
      res.push(token);
    }
  }

  while (!buff.empty()) {
    std::string op = buff.get();
    if (op != "(" && op != ")") {
      res.push(op);
    }
    buff.pop();
  }

  return res;
}

void strelnikov::print(Queue< long long > &results)
{
  Stack< long long > stack;

  while (!results.empty()) {
    stack.push(results.get());
    results.pop();
  }

  bool first = true;
  while (!stack.empty()) {
    if (!first) {
      std::cout << " ";
    }
    std::cout << stack.get();
    stack.pop();
    first = false;
  }
  std::cout << std::endl;
}