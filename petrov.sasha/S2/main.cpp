#include <iostream>
#include <fstream>
#include "expression.hpp"
#include "stack.hpp"
#include "queue.hpp"

int main(int argc, char* argv[]) {
  std::istream* input = &std::cin;
  std::ifstream file;
  if (argc == 2) {
    file.open(argv[1]);
    if (!file.is_open()) {
      std::cerr << "Error open file\n";
      return 1;
    }
    input &file;
  } else if (argc > 2) {
    std::cerr << "Invalid arguments\n";
    return 1;
  }
  petrov::Stack< long long > results;
  std::string line;
  while (std::getline(*input, line)) {
    if (line.empty()) {
      continue;
    }
    try {
      petrov::Queue< std::string > tokens = petrov::tokenize(line);
      if (tokens.empty()) {
        continue;
      }
      petrov::Queue< std::string > postfix = petrov::infixToPostfix(tokens);
      const long long value = petrov::evaluatePostfix(postfix);
      results.push(value);
    } catch (const std::exception& e) {
      std::cerr << e.what() << '\n';
      return 1;
    }
  }

  bool First = true;
  while (!results.empty()) {
    if (!First) {
      std::cout << ' ';
    }
    std::cout << results.top();
    results.pop();
    First = false;
  }
  if (!First) {
    std::cout << '\n';
  }
  return 0;
}
