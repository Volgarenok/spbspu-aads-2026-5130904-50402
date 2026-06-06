#include <fstream>
#include <iostream>
#include <limits>
#include "BSTree.hpp"
#include "comands.hpp"

int main(int argc, char *argv[])
{
  if (argc != 2) {
    std::cerr << "wrong amoung of arguments";
    return 1;
  }

  std::ifstream file;
  file.open(argv[1]);
  if (!file.is_open()) {
    std::cerr << "failed to open file";
    return 1;
  }

  Datasets datasets;
  std::string name;
  while (file >> name) {
    Dataset dataset;
    long long key = 0;
    std::string value;
    while (file >> key) {
      if (file >> value) {
        dataset.push(key, value);
      } else {
        std::cerr << "missing value";
        return 1;
      }
    }
    if (!(file.eof())) {
      file.clear();
    }
    datasets.push(name, dataset);
  }

  file.close();

  using cmd_t = void (*)(std::istream &, std::ostream &, Datasets &);
  saldaev::BSTree< std::string, cmd_t > commands;
  commands.push("print", saldaev::handlePrint);
  commands.push("complement", saldaev::handleComplement);
  commands.push("intersect", saldaev::handleIntersect);
  commands.push("union", saldaev::handleUnion);

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
