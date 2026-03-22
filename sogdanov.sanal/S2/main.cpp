#include <fstream>
#include <iostream>
#include "process.hpp"
int main(int argc, char * argv[]) {
  std::istream * in = & std::cin;
  std::ifstream file;
  if (argc == 2) {
    file.open(argv[1]);
    if (!file) {
      std::cerr << "Cannot open file\n";
      return 1;
    }
    in = & file;
  }
  sogdanov::Stack<long long> res;
  try {
    sogdanov::input(*in, res);
    sogdanov::output(std::cout, res);
  } catch (...) {
    std::cerr << e.what() << '\n';
    return 2;
  }
}
