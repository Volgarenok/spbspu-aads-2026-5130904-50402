#include <fstream>
#include <iostream>
#include <istream>

int main(int argc, char **argv)
{
  if (argc > 2) {
    std::cerr << "bad num of arguments";
    return 1;
  }
  std::istream *input = &std::cin;
  if (argc == 2) {
    std::ifstream file;
    file.open(argv[1]);
    if (!file.is_open()) {
      std::cerr << "bad file";
      return 1;
    }
    input = &file;
  }
}
