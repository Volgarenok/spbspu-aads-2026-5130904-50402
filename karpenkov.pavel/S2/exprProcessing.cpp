#include <iostream>
#include "queue.hpp"
#include "stack.hpp"

using queueExpr = karpenkov::Queue< std::string >;

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
