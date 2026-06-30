#include <cctype>
#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include "queue.hpp"
#include "stack.hpp"

namespace alisov
{
  int getPriority(const std::string &op)
  {
    if (op == "^")
      return 1;
    if (op == "+" || op == "-")
      return 2;
    if (op == "*" || op == "/" || op == "%")
      return 3;
    return 0;
  }

  bool isOperation(const std::string &s)
  {
    return s == "^" || s == "*" || s == "/" || s == "%" || s == "+" || s == "-";
  }

  Queue< std::string > split(const std::string &expression)
  {
    Queue< std::string > res;
    std::string current;
    for (char ch : expression) {
      if (ch == ' ') {
        if (!current.empty()) {
          res.push(current);
          current.clear();
        }
      } else {
        current += ch;
      }
    }
    if (!current.empty()) {
      res.push(current);
    }
    return res;
  }

  Queue< std::string > toPostfix(Queue< std::string > &infixTokens)
  {
    Queue< std::string > outputQueue;
    Stack< std::string > operatorStack;

    while (!infixTokens.empty()) {
      std::string val = infixTokens.get();
      if (val == "(") {
        operatorStack.push(val);
      } else if (val == ")") {
        std::string op = operatorStack.get();
        while (isOperation(op)) {
          outputQueue.push(op);
          operatorStack.pop();
          op = operatorStack.get();
        }
        if (operatorStack.get() == "(") {
          operatorStack.pop();
        }
      } else if (isOperation(val)) {
        if (!operatorStack.empty()) {
          std::string op = operatorStack.get();
          while (op != "(" && getPriority(op) >= getPriority(val)) {
            outputQueue.push(op);
            operatorStack.pop();
            if (operatorStack.empty()) {
              break;
            }
            op = operatorStack.get();
          }
        }
        operatorStack.push(val);
      } else {
        outputQueue.push(val);
      }
      infixTokens.pop();
    }

    while (!operatorStack.empty()) {
      outputQueue.push(operatorStack.get());
      operatorStack.pop();
    }

    return outputQueue;
  }

  long long applyOp(long long a, long long b, const std::string &op)
  {
    if (op == "^") {
      return a ^ b;
    }
    if (op == "+") {
      if ((b > 0 && a > std::numeric_limits< long long >::max() - b)
          || (b < 0 && a < std::numeric_limits< long long >::min() - b)) {
        throw std::overflow_error("Addition overflow");
      }
      return a + b;
    }
    if (op == "-") {
      if ((b > 0 && a < std::numeric_limits< long long >::min() + b)
          || (b < 0 && a > std::numeric_limits< long long >::max() + b)) {
        throw std::underflow_error("Subtraction overflow");
      }
      return a - b;
    }
    if (op == "*") {
      if (a > 0 && b > 0 && a > std::numeric_limits< long long >::max() / b) {
        throw std::overflow_error("Multiplication overflow");
      }
      if (a > 0 && b < 0 && b < std::numeric_limits< long long >::min() / a) {
        throw std::underflow_error("Multiplication underflow");
      }
      if (a < 0 && b > 0 && a < std::numeric_limits< long long >::min() / b) {
        throw std::underflow_error("Multiplication underflow");
      }
      if (a < 0 && b < 0 && (a == -1 || b == -1 || a < std::numeric_limits< long long >::max() / b)) {
        throw std::overflow_error("Multiplication overflow");
      }
      return a * b;
    }
    if (op == "/") {
      if (b == 0) {
        throw std::runtime_error("Division by zero");
      }
      if (a == std::numeric_limits< long long >::min() && b == -1) {
        throw std::overflow_error("Division overflow");
      }
      return a / b;
    }
    if (op == "%") {
      if (b == 0) {
        throw std::runtime_error("Division by zero");
      }
      long long res = a % b;
      if (res < 0 && b > 0) {
        res += b;
      }
      if (res > 0 && b < 0) {
        res += b;
      }
      return res;
    }
    throw std::invalid_argument("Unknown operator");
  }

  long long evaluatePostfix(Queue< std::string > &postfixTokens)
  {
    Stack< long long > operandStack;

    while (!postfixTokens.empty()) {
      std::string token = postfixTokens.get();
      postfixTokens.pop();

      if (!isOperation(token)) {
        operandStack.push(std::stoll(token));
      } else {
        if (operandStack.empty()) {
          throw std::runtime_error("Invalid expression structure");
        }
        long long b = operandStack.get();
        operandStack.pop();

        if (operandStack.empty()) {
          throw std::runtime_error("Invalid expression structure");
        }
        long long a = operandStack.get();
        operandStack.pop();

        long long result = applyOp(a, b, token);
        operandStack.push(result);
      }
    }

    if (operandStack.empty()) {
      throw std::runtime_error("Invalid expression structure");
    }

    long long finalResult = operandStack.get();
    operandStack.pop();

    if (!operandStack.empty()) {
      throw std::runtime_error("Invalid expression structure");
    }

    return finalResult;
  }

  void processExpressions(std::istream &input)
  {
    std::string line;
    Stack< long long > results;

    while (std::getline(input, line)) {
      if (line.empty()) {
        continue;
      }
      Queue< std::string > tokens = split(line);
      if (tokens.empty()) {
        continue;
      }
      Queue< std::string > postfix = toPostfix(tokens);
      long long res = evaluatePostfix(postfix);
      results.push(res);
    }

    bool first = true;
    while (!results.empty()) {
      if (!first) {
        std::cout << " ";
      }
      std::cout << results.get();
      results.pop();
      first = false;
    }
    std::cout << "\n";
  }

}

int main(int argc, char *argv[])
{
  if (argc > 2) {
    std::cerr << "Usage: " << argv[0] << " {filename}\n";
    return 1;
  }

  try {
    if (argc == 2) {
      std::ifstream file(argv[1]);
      if (!file.is_open()) {
        std::cerr << "Cannot open file: " << argv[1] << "\n";
        return 1;
      }
      alisov::processExpressions(file);
    } else {
      alisov::processExpressions(std::cin);
    }
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }
}
