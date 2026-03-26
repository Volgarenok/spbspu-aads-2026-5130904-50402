#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include "calculator.hpp"
#include "stack.hpp"

int main(int argc, char *argv[])
{
  std::istream *input = &std::cin;
  std::ifstream fileStream;

  if (argc == 2) {
    fileStream.open(argv[1]);
    if (!fileStream.is_open()) {
      return 1;
    }
    input = &fileStream;
  } else if (argc > 2) {
    return 1;
  }

  pozdnyakov::Stack< long long > results;
  std::string line;

  while (std::getline(*input, line)) {
    try {
      long long currentResult = pozdnyakov::evaluateExpression(line);
      results.push(currentResult);
    } catch (const std::exception &) {
      return 1;
    }
  }

  bool isFirst = true;
  while (!results.empty()) {
    if (!isFirst) {
      std::cout << " ";
    }
    std::cout << results.top();
    results.pop();
    isFirst = false;
  }

  std::cout << "\n";

  return 0;
}
