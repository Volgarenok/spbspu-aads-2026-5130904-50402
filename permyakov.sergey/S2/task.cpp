#ifndef TASK_CPP
#define TASK_CPP
#include "task.hpp"

namespace permyakov
{
  Queue< Queue< std::string > > input(std::istream & is)
  {
    Queue< Queue< std::string > > result;
    std::string line;
    while (std::getline(is, line)) {
      if (line.empty()) {
        continue;
      }
      Queue< std::string > exc;
      std::string element;
      for (size_t i = 0; i < line.length(); ++i) {
        char c = line[i];
        if (c == ' ') {
          exc.push(element);
          element.clear();
        } else {
          element += c;
        }
      }
      exc.push(element);
      result.push(exc);
    }
    return result;
  }

  void output(std::ostream & out, Stack< long long > stk)
  {
    out << stk.top();
    stk.pop();
    while (!stk.isEmpty()) {
      out << ' '<< stk.top();
      stk.pop();
    }
  }
}

#endif
