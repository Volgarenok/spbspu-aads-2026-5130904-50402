#include "exprProcessing.hpp"
#include "queue.hpp"
#include "stack.hpp"
#include <cctype>
#include <iostream>
#include <limits>
#include <stdexcept>

bool isNumber(const std::string &element) { return std::isdigit(element[0]); }

bool isValidOperator(const std::string &element) {
  if (element == "+" || element == "-" || element == "*" || element == "/" ||
      element == "%" || element == "**") {
    return true;
  }
  return false;
}
size_t priority(const std::string &operation) {
  if (operation == "+" || operation == "-") {
    return 1;
  } else if (operation == "/" || operation == "*" || operation == "%") {
    return 2;
  } else if (operation == "**") {
    return 3;
  }
  return 0;
}

karpenkov::Stack<queueExpr> toPostfix(karpenkov::Stack<queueExpr> expressions) {
  karpenkov::Stack<queueExpr> postFix;
  while (!expressions.empty()) {
    karpenkov::Queue<std::string> resultExpr;
    karpenkov::Stack<std::string> tempStep;
    karpenkov::Queue<std::string> curExpr = expressions.top();
    while (!curExpr.empty()) {
      std::string element = curExpr.front();
      if (element == "(") {
        tempStep.push(element);
      }

      else if (element == ")") {
        while (!tempStep.empty() && tempStep.top() != "(") {
          resultExpr.push(tempStep.top());
          tempStep.pop();
        }
        tempStep.pop();
      }

      else if (isNumber(element)) {
        resultExpr.push(element);
      }

      else if (isValidOperator(element)) {
        while (!tempStep.empty() && tempStep.top() != "(" &&
               priority(tempStep.top()) >= priority(element)) {
          resultExpr.push(tempStep.top());
          tempStep.pop();
        }
        tempStep.push(element);
      }
      curExpr.pop();
    }
    while (!tempStep.empty()) {
      resultExpr.push(tempStep.top());
      tempStep.pop();
    }
    expressions.pop();
    postFix.push(resultExpr);
  }
  return postFix;
}

karpenkov::Stack<queueExpr> inputCLI(std::istream &in) {
  karpenkov::Stack<queueExpr> exprs;
  std::string line;
  while (std::getline(in, line)) {
    if (!line.empty()) {
      karpenkov::Queue<std::string> curExpr;
      std::string element;
      for (char a : line) {
        if (a != ' ') {
          element += a;
        } else {
          curExpr.push(element);
          element.clear();
        }
      }
      if (!element.empty()) {
        curExpr.push(element);
      }
      exprs.push(curExpr);
    }
  }
  return exprs;
}
long long eval(long long a, long long b, const std::string &operation) {
  if (operation == "+") {
    if (b > 0 && a > std::numeric_limits<long long>::max() - b) {
      throw std::overflow_error("Overflow");
    }
    if (b < 0 && a < std::numeric_limits<long long>::min() - b) {
      throw std::overflow_error("underflow");
    }
    return a + b;
  } else if (operation == "-") {
    if (b < 0 && a > std::numeric_limits<long long>::max() + b) {
      throw std::overflow_error("overflow");
    }
    if (b > 0 && a < std::numeric_limits<long long>::min() + b) {
      throw std::overflow_error("underflow");
    }
    return a - b;
  } else if (operation == "*") {
    if (a != 0 && b != 0) {
      if (a > std::numeric_limits<long long>::max() / b) {
        throw std::overflow_error("overflow");
      }
      if (a < std::numeric_limits<long long>::min() / b) {
        throw std::overflow_error("underflow");
      }
    }
    return a * b;
  } else if (operation == "/") {
    if (b == 0) {
      throw std::runtime_error("divide by 0");
    }
    if (a == std::numeric_limits<long long>::min() && b == -1) {
      throw std::overflow_error("overflow");
    }
    return a / b;
  } else if (operation == "%") {
    if (b == 0) {
      throw std::runtime_error("divide by 0");
    }
    long long result = a % b;
    if (result < 0) {
      result += std::abs(b);
    }

    return result;
  } else if (operation == "**") {
    long long result = 1;
    for (long long i = 0; i < b; ++i) {
      result *= a;
    }
    return result;
  }
  throw std::runtime_error("Unkown operation");
}
karpenkov::Stack<long long>
calculateExpr(karpenkov::Stack<queueExpr> &postfixExpr) {
  karpenkov::Stack<long long> res;
  while (!postfixExpr.empty()) {
    queueExpr curExpr = postfixExpr.top();
    karpenkov::Stack<long long> calculateStack;
    while (!curExpr.empty()) {
      std::string element = curExpr.front();
      if (isNumber(element)) {
        try {
          calculateStack.push(std::stoll(element));
        } catch (...) {
          throw std::runtime_error("element is not a number: " + element);
        }
      } else {
        if (calculateStack.empty()) {
          throw std::runtime_error("Not enough operands");
        }
        long long b = calculateStack.top();
        calculateStack.pop();
        if (calculateStack.empty()) {
          throw std::runtime_error("Not enough operands");
        }
        long long a = calculateStack.top();
        calculateStack.pop();
        calculateStack.push(eval(a, b, element));
      }
      curExpr.pop();
    }
    res.push(calculateStack.top());
    postfixExpr.pop();
  }
  return res;
}
void queueOutput(std::ostream &out, karpenkov::Stack<long long> result) {
  bool isFirst = true;
  while (!result.empty()) {
    if (isFirst) {
      out << result.top();
      isFirst = false;
    } else {
      out << ' ' << result.top();
    }
    result.pop();
  }
}
