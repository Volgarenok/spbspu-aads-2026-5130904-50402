#include <iostream>
#include "exprProcessing.cpp"
#include "queue.hpp"

int main(int argc, char **argv)
{
  karpenkov::Stack< queueExpr > expression;
  if (argc == 1) {
    expression = inputCLI(std::cin);
    expression = toPostfix(expression);
    queueOutput(std::cout, calculateExpr(expression));
  }
}
