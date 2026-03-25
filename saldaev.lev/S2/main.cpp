#include "queue.hpp"
#include "stack.hpp"
#include <fstream>
#include <iostream>

saldaev::Queue< saldaev::Queue< std::string > > parse(saldaev::Queue< std::string > rawLines);
saldaev::Queue< saldaev::Queue< std::string > >
toPostfixNotation(saldaev::Queue< saldaev::Queue< std::string > > parsedLines);

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

  saldaev::Queue< saldaev::Queue< std::string > > parsedLines = parse(rawLines);

  saldaev::Queue< saldaev::Queue< std::string > > rearrangedLines;
  try {
    rearrangedLines = toPostfixNotation(parsedLines);
  } catch (const std::logic_error &e) {
    std::cerr << e.what() << '\n';
    return 1;
  }

  while (!rearrangedLines.empty()) {
    saldaev::Queue< std::string > tokens = rearrangedLines.front();
    rearrangedLines.pop();

    while (!tokens.empty()) {
      std::cout << tokens.front() << " ";
      tokens.pop();
    }
    std::cout << '\n';
  }
}

saldaev::Queue< saldaev::Queue< std::string > > parse(saldaev::Queue< std::string > rawLines)
{
  saldaev::Queue< saldaev::Queue< std::string > > parsedLines;
  while (!rawLines.empty()) {
    saldaev::Queue< std::string > tokens;
    std::string currLine = rawLines.front();
    rawLines.pop();

    size_t start = 0;
    size_t index = currLine.find(' ');
    std::string token = "";
    while (index != std::string::npos) {
      token = currLine.substr(start, index - start);
      tokens.push(token);
      start = index + 1;
      index = currLine.find(' ', start);
    }
    token = currLine.substr(start);
    tokens.push(token);

    parsedLines.push(tokens);
  }
  return parsedLines;
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

saldaev::Queue< saldaev::Queue< std::string > >
toPostfixNotation(saldaev::Queue< saldaev::Queue< std::string > > parsedLines)
{
  saldaev::Queue< saldaev::Queue< std::string > > rearrangedLines;
  while (!parsedLines.empty()) {
    saldaev::Queue< std::string > currLine = parsedLines.front();
    parsedLines.pop();

    saldaev::Queue< std::string > newLine;
    saldaev::Stack< std::string > stack;
    std::string token = "";
    while (!currLine.empty()) {
      token = currLine.front();
      currLine.pop();
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
    rearrangedLines.push(newLine);
  }
  return rearrangedLines;
}
