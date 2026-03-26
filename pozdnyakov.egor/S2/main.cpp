#include <cctype>
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
      std::cerr << "Error: Could not open file " << argv[1] << "\n";
      return 1;
    }
    input = &fileStream;
  } else if (argc > 2) {
    std::cerr << "Error: Too many arguments. Usage: ./lab [filename]\n";
    return 1;
  }

  pozdnyakov::Stack< long long > results;
  std::string line;

  while (std::getline(*input, line)) {
    bool isEmpty = true;

    for (size_t i = 0; i < line.length(); ++i) {
      if (!std::isspace(line[i])) {
        isEmpty = false;
        break;
      }
    }

    if (isEmpty) {
      continue;
    }

    try {
      long long currentResult = pozdnyakov::evaluateExpression(line);
      results.push(currentResult);
    } catch (const std::exception &e) {
      std::cerr << "Error: " << e.what() << "\n";
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

  if (!isFirst) {
    std::cout << "\n";
  }

  return 0;
}
