#include <cctype>
#include <iostream>
#include "queue.hpp"
#include "stack.hpp"

using queueExpr = karpenkov::Queue< std::string >;
using queueResult = karpenkov::Queue< int >;

bool isNumber(std::string &element)
{
  return std::isdigit(element[0]);
}

bool isValidOperator(std::string &element)
{
  if (element == "+" || element == "-" || element == "*" || element == "/") {
    return true;
  }
  return false;
}
size_t priority(std::string &operation)
{
  if (operation == "+" || operation == "-") {
    return 1;
  } else if (operation == "/" || operation == "*") {
    return 2;
  }
}

karpenkov::Stack< queueExpr > toPostfix(karpenkov::Stack< queueExpr > expressions)
{
  karpenkov::Stack< queueExpr > postFix;
  while (!expressions.empty()) {
    karpenkov::Queue< std::string > resultExpr;
    karpenkov::Stack< std::string > tempStep;
    karpenkov::Queue< std::string > curExpr = expressions.top();
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
        while (!tempStep.empty() && tempStep.top() != "(" && priority(tempStep.top()) >= priority(element)) {
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

karpenkov::Stack< queueExpr > inputCLI(std::istream &in)
{
  karpenkov::Stack< queueExpr > exprs;
  std::string line;
  while (std::getline(in, line)) {
    if (!line.empty()) {
      karpenkov::Queue< std::string > curExpr;
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
int eval(int &a, int &b, std::string &operation)
{
  if (operation == "+") {
    return a + b;
  } else if (operation == "-") {
    return a - b;
  } else if (operation == "*") {
    return a * b;
  } else if (operation == "/") {
    return a / b;
  }
}
karpenkov::Stack< int > calculateExpr(karpenkov::Stack< queueExpr > &postfixExpr)
{
  karpenkov::Stack< int > res;
  while (!postfixExpr.empty()) {
    queueExpr curExpr = postfixExpr.top();
    karpenkov::Stack< int > calculateStack;
    while (!curExpr.empty()) {
      std::string element = curExpr.front();
      if (isNumber(element)) {
        calculateStack.push(std::stoi(element));
      } else {
        int b = calculateStack.top();
        calculateStack.pop();
        int a = calculateStack.top();
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
void queueOutput(std::ostream &out, karpenkov::Stack< int > result)
{
  while (!result.empty()) {
    out << result.top() << ' ';
    result.pop();
  }
}
