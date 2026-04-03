#include <cctype>
#include <iostream>
#include "queue.hpp"
#include "stack.hpp"

using queueExpr = karpenkov::Queue< std::string >;

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
      size_t pr;
      std::string element = curExpr.front();
      if (element == "(") {
        tempStep.push(element);
      } else if (element == ")") {
        while (!tempStep.empty()) {
          if (tempStep.top() != "(") {
            while (pr < priority(tempStep.top())) {
              resultExpr.push(tempStep.top());
              pr = priority(tempStep.top());
            }
          }
          tempStep.pop();
        }
      } else if (isNumber(element)) {
        resultExpr.push(element);
      } else if (isValidOperator(element)) {
        pr = priority(element);
        tempStep.push(element);
      }
      curExpr.pop();
    }
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
