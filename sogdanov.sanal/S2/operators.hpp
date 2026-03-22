#ifndef OPERATORS_HPP
#define OPERATORS_HPP
#include <string>
#include <cstddef>
#include "queue.hpp"
#include "stack.hpp"
namespace sogdanov {
  size_t getOp(char op);
  bool isOp(char c);
  long long applyOp(char op, long long a, long long b);
  Queue<std::string> convertToPostfix(const std::string& line);
  long long calculatePostfix(Queue<std::string>& postfix);
  long long evaluate(const std::string& line);
}
#endif
