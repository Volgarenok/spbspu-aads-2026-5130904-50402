#ifndef EXPRESSION_CONVERTER_HPP
#define EXPRESSION_CONVERTER_HPP
#include "expression_utils.hpp"
#include "queue.hpp"
#include "stack.hpp"
#include <stdexcept>
#include <string>

namespace saldaev
{
  Queue< std::string > parse(std::string rawLine)
  {
    Queue< std::string > tokens;
    size_t start = 0;
    size_t index = rawLine.find(' ');
    std::string token = "";
    while (index != std::string::npos) {
      token = rawLine.substr(start, index - start);
      tokens.push(token);
      start = index + 1;
      index = rawLine.find(' ', start);
    }
    token = rawLine.substr(start);
    tokens.push(token);

    return tokens;
  }

  Queue< std::string > toPostfixNotation(Queue< std::string > parsedLine)
  {
    Queue< std::string > newLine;
    Stack< std::string > stack;
    std::string token = "";
    while (!parsedLine.empty()) {
      token = parsedLine.front();
      parsedLine.pop();

      if (isOperator(token)) {
        while (!stack.empty() && getPriority(stack.top()) >= getPriority(token)) {
          newLine.push(stack.top());
          stack.pop();
        }
        stack.push(token);
      } else if (token == "(") {
        stack.push(token);
      } else if (token == ")") {
        while (!stack.empty() && stack.top() != "(") {
          newLine.push(stack.top());
          stack.pop();
        }
        if (stack.empty()) {
          throw std::logic_error("invalid (unpaired ')')");
        }
        stack.pop();
      } else {
        newLine.push(token);
      }
    }
    while (!stack.empty()) {
      if (stack.top() == "(") {
        throw std::logic_error("invalid (unpaired '(')");
      }
      newLine.push(stack.top());
      stack.pop();
    }
    return newLine;
  }
}

#endif
