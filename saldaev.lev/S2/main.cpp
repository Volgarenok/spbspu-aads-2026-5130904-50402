#include "queue.hpp"
#include <fstream>
#include <iostream>

int main(int argc, char *argv[])
{
  if (argc > 2) {
    return 1;
  }

  std::ifstream file;
  if (argc == 2) {
    file.open(argv[1]);
    if (!file.is_open()) {
      return 1;
    }
  }
  std::istream &input = (argc == 2) ? file : std::cin;

  saldaev::Queue< std::string > lines;
  std::string line;
  while (std::getline(input, line)) {
    if (!line.empty()) {
      lines.push(line);
    }
  }
}
