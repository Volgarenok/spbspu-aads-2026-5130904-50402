#include <fstream>
#include <iostream>
#include <limits>
#include "bstree.hpp"
#include "cmds.hpp"
namespace dirko
{
  bool isInt(const std::string &str)
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
    std::cerr << "bad num of args\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file.is_open()) {
    std::cerr << "Cannot open file\n";
    return 1;
  }

  dirko::Datasets datasets;
  std::string word;
  std::string currentDataset;
  while (file >> word) {
    if (dirko::isInt(word)) {
      int key = std::stoi(word);
      std::string value;
      if (file >> value) {
        datasets.get(currentDataset).push(key, value);
      }
    } else {
      currentDataset = word;
      try {
        datasets.get(currentDataset);
      } catch (const std::out_of_range &) {
        datasets.push(currentDataset, dirko::Dataset{});
      }
    }
  }
  file.close();

  using cmd_t = void (*)(std::istream &, std::ostream &, dirko::Datasets &);
  dirko::BSTree< std::string, cmd_t > commands = {{"print", dirko::cmdPrint},
                                                  {"complement", dirko::cmdComplement},
                                                  {"intersect", dirko::cmdIntersect},
                                                  {"union", dirko::cmdUnion}};
  std::string cmd;
  while (std::cin >> cmd) {
    try {
      commands.get(cmd)(std::cin, std::cout, datasets);
    } catch (const std::exception &) {
      std::cout << "<INVALID COMMAND>" << '\n';
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}
