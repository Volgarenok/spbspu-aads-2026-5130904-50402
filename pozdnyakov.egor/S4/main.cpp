#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include "bstree.hpp"
#include "commands.hpp"

int main(int argc, char **argv)
{
  if (argc != 2) {
    std::cerr << "Bad num of args\n";
    return 1;
  }

  pozdnyakov::Datasets datasets;
  std::ifstream file(argv[1]);
  if (!file.is_open()) {
    std::cerr << "Cannot open file\n";
    return 1;
  }

  std::string line;
  while (std::getline(file, line)) {
    if (line.empty()) {
      continue;
    }

    std::size_t start = 0;
    while (start < line.size() && std::isspace(line[start])) {
      ++start;
    }
    if (start == line.size()) {
      continue;
    }

    std::size_t end = start;
    while (end < line.size() && !std::isspace(line[end])) {
      ++end;
    }

    const std::string currentDataset = line.substr(start, end - start);
    try {
      datasets.get(currentDataset);
    } catch (const std::out_of_range &) {
      datasets.push(currentDataset, pozdnyakov::Dataset{});
    }

    start = end;
    while (true) {
      while (start < line.size() && std::isspace(line[start])) {
        ++start;
      }
      if (start == line.size()) {
        break;
      }
      end = start;
      while (end < line.size() && !std::isspace(line[end])) {
        ++end;
      }
      const std::string keyStr = line.substr(start, end - start);
      start = end;

      while (start < line.size() && std::isspace(line[start])) {
        ++start;
      }
      if (start == line.size()) {
        break;
      }
      end = start;
      while (end < line.size() && !std::isspace(line[end])) {
        ++end;
      }
      const std::string valStr = line.substr(start, end - start);
      start = end;

      if (pozdnyakov::isInteger(keyStr)) {
        const int key = std::stoi(keyStr);
        datasets.get(currentDataset).push(key, valStr);
      }
    }
  }
  file.close();

  using cmd_t = void (*)(std::istream &, std::ostream &, pozdnyakov::Datasets &);
  pozdnyakov::BSTree< std::string, cmd_t > commands;

  commands.push("print", pozdnyakov::cmdPrint);
  commands.push("complement", pozdnyakov::cmdComplement);
  commands.push("intersect", pozdnyakov::cmdIntersect);
  commands.push("union", pozdnyakov::cmdUnion);

  std::string cmdName;
  while (std::cin >> cmdName) {
    try {
      cmd_t commandFunc = commands.get(cmdName);
      commandFunc(std::cin, std::cout, datasets);
    } catch (const std::out_of_range &) {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  return 0;
}
