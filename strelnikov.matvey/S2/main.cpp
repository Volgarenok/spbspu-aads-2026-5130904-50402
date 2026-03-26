#include <fstream>
#include <iostream>
#include <stdexcept>
#include "datastructs.hpp"
#include "funcs.hpp"

int main(int argc, char *argv[])
{
  try {
    std::istream *in = &std::cin;
    std::ifstream file;

    if (argc > 1) {
      file.open(argv[1]);
      if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << argv[1] << std::endl;
        return 1;
      }
      in = &file;
    }

    strelnikov::Queue< strelnikov::Queue< std::string > > expressions = strelnikov::input(*in);

    strelnikov::Queue< long long > results;
    while (!expressions.empty()) {
      strelnikov::Queue< std::string > expr = expressions.get();
      expressions.pop();

      strelnikov::Queue< std::string > postfix = strelnikov::convertToPostfix(expr);

      long long result = strelnikov::calc(postfix);
      results.push(result);
    }

    strelnikov::print(results);

    if (file.is_open()) {
      file.close();
    }

    return 0;
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
}