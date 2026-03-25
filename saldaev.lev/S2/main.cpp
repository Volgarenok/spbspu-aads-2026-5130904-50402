#include "queue.hpp"
#include "stack.hpp"
#include <fstream>
#include <iostream>

saldaev::Queue< std::string > parse(std::string rawLine);
saldaev::Queue< std::string > toPostfixNotation(saldaev::Queue< std::string > parsedLine);

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

  saldaev::Queue< std::string > rawLines;
  std::string currLine;
  while (std::getline(input, currLine)) {
    if (!currLine.empty()) {
      rawLines.push(currLine);
    }
  }

  saldaev::Queue< saldaev::Queue< std::string > > parsedLines;
  while (!rawLines.empty()) {
    parsedLines.push(parse(rawLines.front()));
    rawLines.pop();
  }

  saldaev::Queue< saldaev::Queue< std::string > > postfixedLines;
  while (!parsedLines.empty()) {
    try {
      postfixedLines.push(toPostfixNotation(parsedLines.front()));
    } catch (const std::logic_error &e) {
      std::cerr << e.what() << '\n';
      return 1;
    }
    parsedLines.pop();
  }

  while (!postfixedLines.empty()) {
    saldaev::Queue< std::string > tokens = postfixedLines.front();
    postfixedLines.pop();

    while (!tokens.empty()) {
      std::cout << tokens.front() << " ";
      tokens.pop();
    }
    std::cout << '\n';
  }
}

saldaev::Queue< std::string > parse(std::string rawLine)
{
  saldaev::Queue< std::string > tokens;
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

bool isNumber(std::string token)
{
  for (size_t i = 0; i < token.size(); ++i) {
    if (!std::isdigit(token[i])) {
      return false;
    }
  }
  return true;
}

bool isOperator(std::string token)
{
  if (token == "+" || token == "-" || token == "*" || token == "/" || token == "%" || token == "!") {
    return true;
  }
  return false;
}

int getPriority(const std::string &op)
{
  if (op == "+" || op == "-") {
    return 1;
  }
  if (op == "*" || op == "/" || op == "%") {
    return 2;
  }
  return 0;
}

saldaev::Queue< std::string > toPostfixNotation(saldaev::Queue< std::string > parsedLine)
{
  saldaev::Queue< std::string > newLine;
  saldaev::Stack< std::string > stack;
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
