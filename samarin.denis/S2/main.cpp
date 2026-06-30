#include <iostream>
#include <fstream>
#include <string>
#include "calculator.hpp"
#include "stack.hpp"

int main(int argc, char* argv[])
{
  std::ifstream file;
  std::istream* input = &std::cin;
  if (argc > 1) {
    file.open(argv[1]);
    if (!file.is_open()) {
      std::cerr << "cannot open input file\n";
      return 1;
    }
    input = &file;
  }

  samarin::Stack< long long > results;
  try {
    std::string line;
    while (std::getline(*input, line)) {
      if (line.find_first_not_of(" \t\r") == std::string::npos) {
        continue;
      }
      results.push(samarin::calculateExpression(line));
    }
  } catch (const std::exception& e) {
    std::cerr << e.what() << "\n";
    return 2;
  }

  bool first = true;
  while (!results.empty()) {
    if (!first) {
      std::cout << " ";
    }
    std::cout << results.drop();
    first = false;
  }
  std::cout << "\n";
  return 0;
}
