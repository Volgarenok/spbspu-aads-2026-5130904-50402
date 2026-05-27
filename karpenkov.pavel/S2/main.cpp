#include <exception>
#include <iostream>
#include <fstream>
#include "exprProcessing.hpp"
#include "queue.hpp"

int main(int argc, char **argv)
{
  try {
    karpenkov::Stack< queueExpr > expression;
    if (argc == 1) {
      expression = inputCLI(std::cin);
    }

    else if (argc == 2) {
      std::ifstream file(argv[1]);
      if (!file.is_open()) {
        std::cerr << "Reading file error";
        return 1;
      }
      expression = inputCLI(file);
    }
    karpenkov::Stack< queueExpr > postFix = toPostfix(expression);
    queueOutput(std::cout, calculateExpr(postFix));
    std::cout << '\n';
  } catch (std::exception &e) {
    std::cerr << e.what() << "\n";
    return 1;
  }
  return 0;
}
