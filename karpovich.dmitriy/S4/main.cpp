#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include "commands.hpp"

namespace karpovich
{
  bool isInteger(const std::string &str)
  {
    if (str.empty()) {
      return false;
    }
    size_t start = 0;
    if (str[0] == '-' || str[0] == '+') {
      start = 1;
    }
    if (start == str.size()) {
      return false;
    }
    for (size_t i = start; i < str.size(); ++i) {
      if (str[i] < '0' || str[i] > '9') {
        return false;
      }
    }
    return true;
  }
}

int main(int argc, char **argv)
{
  if (argc != 2) {
    std::cerr << "bad num of args" << '\n';
    return 1;
  }

  karpovich::Datasets datasets;
  std::ifstream file(argv[1]);
  if (!file.is_open()) {
    std::cerr << "Cannot open file" << '\n';
    return 1;
  }

  std::string token;
  std::string currentDataset;
  while (file >> token) {
    if (karpovich::isInteger(token)) {
      int key = std::stoi(token);
      std::string value;
      if (file >> value) {
        datasets.at(currentDataset).push(key, value);
      }
    } else {
      currentDataset = token;
      try {
        datasets.at(currentDataset);
      } catch (const std::out_of_range &) {
        datasets.push(currentDataset, karpovich::Dataset{});
      }
    }
  }
  file.close();

  using cmd_t = void (*)(std::istream &, std::ostream &, karpovich::Datasets &);
  karpovich::BSTree< std::string, cmd_t > commands;

  commands.push("print", karpovich::cmdPrint);
  commands.push("complement", karpovich::cmdComplement);
  commands.push("intersect", karpovich::cmdIntersect);
  commands.push("union", karpovich::cmdUnion);

  std::string cmd;
  while (std::cin >> cmd) {
    try {
      commands.at(cmd)(std::cin, std::cout, datasets);
    } catch (const std::exception &) {
      std::cout << "<INVALID COMMAND>" << '\n';
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}
