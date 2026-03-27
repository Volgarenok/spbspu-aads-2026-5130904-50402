#include "expression_converter.hpp"
#include "queue.hpp"
#include "stack.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

namespace saldaev
{
  long long evaluate(saldaev::Queue< std::string > postfixedLines)
  {
    saldaev::Stack< long long > stack;
    while (!postfixedLines.empty()) {
      std::string token = postfixedLines.front();
      postfixedLines.pop();
      if (isNumber(token)) {
        stack.push(std::stoi(token));
      } else if (isOperator(token)) {
        if (stack.size() < 2) {
          throw std::logic_error("Not enough operands");
        }
        long long op1 = stack.top();
        stack.pop();
        long long op2 = stack.top();
        stack.pop();
        stack.push(apply(token, op2, op1));
      } else {
        throw std::logic_error("Ivalid token");
      }
    }
    if (stack.size() > 1) {
      throw std::logic_error("Redundant operands");
    }
    return stack.top();
  }
}

using saldaev::evaluate;
using saldaev::parse;
using saldaev::toPostfixNotation;

int main(int argc, char *argv[])
{
  if (argc > 2) {
    return 1;
  }

  std::ifstream file;
  if (argc == 2) {
    file.open(argv[1]);
    if (!file.is_open()) {
      return 1;
    }
  }
  std::istream &input = (argc == 2) ? file : std::cin;

  saldaev::Stack< long long > answers;
  std::string currLine;
  while (std::getline(input, currLine)) {
    if (!currLine.empty()) {
      try {
        answers.push(evaluate(toPostfixNotation(parse(currLine))));
      } catch (const std::logic_error &e) {
        std::cerr << e.what() << '\n';
        return 1;
      }
    }
  }

  std::cout << answers.top();
  answers.pop();
  while (!answers.empty()) {
    std::cout << ' ' << answers.top();
    answers.pop();
  }
  std::cout << '\n';
}
