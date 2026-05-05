#include <iostream>
#include <fstream>
#include <string>
#include "eval.hpp"

novikov::Stack< std::string > read_expressions(std::istream &in)
{
  novikov::Stack< std::string > st;
  std::string line;
  while (std::getline(in, line)) {
    if (!line.empty()) {
      st.push(line);
    }
  }
  return st;
}

int main(int argc, char *argv[])
{
  novikov::Stack< std::string > expressions;
  if (argc == 1) {
    try {
      expressions = read_expressions(std::cin);
    } catch (const std::bad_alloc &) {
      std::cerr << "Memory allocation error\n";
      return 2;
    }
  } else if (argc == 2) {
    try {
      std::ifstream file(argv[1]);
      if (!file) {
        std::cerr << "Failed to open file\n";
        return 1;
      }
      expressions = read_expressions(file);
    } catch (const std::bad_alloc &) {
      std::cerr << "Memory allocation error\n";
      return 2;
    }
  } else {
    std::cerr << "Invalid arguments\n";
    return 1;
  }

  while (!expressions.empty()) {
    int result;
    try {
      result = novikov::eval(expressions.front());
    } catch (...) {
      std::cerr << "Evaluation error\n";
      return 3;
    }
    std::cout << result;
    expressions.drop();
    if (!expressions.empty()) {
      std::cout << ' ';
    }
  }
  std::cout << '\n';
}
