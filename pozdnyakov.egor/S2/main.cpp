#include "calculator.hpp"
#include "stack.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <exception>

int main(int argc, char* argv[])
{
  std::istream* in = &std::cin;
  std::ifstream file;

  if (argc == 2) {
    file.open(argv[1]);
    if (!file.is_open()) {
      std::cerr << "Could not open file " << argv[1] << "\n";
      return 1;
    }
    in = &file;
  }
  else if (argc > 2) {
    std::cerr << "Too many arguments.\n";
    return 1;
  }

  pozdnyakov::Stack< long long > results;
  std::string line;

  while (std::getline(*in, line)) {
    bool isEmpty = true;
    for (char c : line) {
      if (!std::isspace(c)) {
        isEmpty = false;
        break;
      }
    }
    if (isEmpty) continue;

    try {
      long long res = pozdnyakov::evaluateExpression(line);
      results.push(res);
    }
    catch (const std::exception& e) {
      std::cerr << "Error: " << e.what() << "\n";
      return 1;
    }
  }

  bool first = true;
  while (!results.empty()) {
    if (!first) {
      std::cout << " ";
    }
    std::cout << results.top();
    results.pop();
    first = false;
  }

  if (!first) {
    std::cout << "\n";
  }

  return 0;
}
