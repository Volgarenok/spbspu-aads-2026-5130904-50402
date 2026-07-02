#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include "BSTree.hpp"
#include "commands.hpp"

namespace alisov
{
  datasets_t parse(std::istream &in);
}

int main(int argc, char *argv[])
{
  if (argc < 2) {
    std::cerr << "Bad args\n";
    return 1;
  }
  std::ifstream in(argv[1]);
  if (!in.is_open()) {
    std::cerr << "Couldn't open the file\n";
    return 1;
  }
  alisov::datasets_t datasets = alisov::parse(in);
  using cmd_t = void (*)(std::ostream &, std::istream &, alisov::datasets_t &);
  alisov::BSTree< std::string, cmd_t, std::less< std::string > > cmds;
  cmds["print"] = alisov::print;
  cmds["complement"] = alisov::complement;
  cmds["intersect"] = alisov::intersect;
  cmds["union"] = alisov::union_;
  std::string cmd;
  while (std::cin >> cmd) {
    try {
      cmds.at(cmd)(std::cout, std::cin, datasets);
    } catch (...) {
      std::cout << "<INVALID COMMAND>\n";
      auto toIgnore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toIgnore, '\n');
    }
  }
  if (!std::cin.eof()) {
    std::cerr << "Bad input\n";
    return 1;
  }
}

alisov::datasets_t alisov::parse(std::istream &in)
{
  datasets_t datasets;
  std::string name;
  while (in >> name) {
    BSTree< int, std::string, std::less< int > > ds;
    int key;
    std::string value;
    while (in >> key >> value) {
      ds.insert(key, value);
    }
    datasets.insert(name, ds);
    if (in.eof()) {
      break;
    }
    in.clear();
  }
  return datasets;
}
