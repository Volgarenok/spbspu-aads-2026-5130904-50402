#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <cctype>
#include "bstree.hpp"
#include "cmd.hpp"

namespace sogdanov
{

  bool isInteger(const std::string &str)
  {
    if (str.empty())
    {
      return false;
    }
    size_t start = 0;
    if (str[0] == '-' || str[0] == '+')
    {
      start = 1;
    }
    if (start == str.length())
    {
      return false;
    }
    for (size_t i = start; i < str.length(); ++i)
    {
      if (!std::isdigit(str[i]))
      {
        return false;
      }
    }
    return true;
  }

}

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    std::cerr << "bad num of args\n";
    return 1;
  }

  sogdanov::Datasets datasets;
  std::ifstream file(argv[1]);

  if (!file.is_open())
  {
    std::cerr << "Cannot open file\n";
    return 1;
  }

  std::string token;
  std::string currentDataset;

  while (file >> token)
  {
    if (sogdanov::isInteger(token))
    {
      int key = std::stoi(token);
      std::string value;
      if (file >> value)
      {
        sogdanov::BSTIterator<std::string, sogdanov::Dataset> it = datasets.find(currentDataset);
        if (it != datasets.end())
        {
          (*it).second.push(key, value);
        }
      }
    }
    else
    {
      currentDataset = token;
      sogdanov::BSTIterator<std::string, sogdanov::Dataset> it = datasets.find(currentDataset);
      if (it == datasets.end())
      {
        sogdanov::Dataset empty_tree;
        datasets.push(currentDataset, empty_tree);
      }
    }
  }
  file.close();

  using cmd_t = void (*)(std::istream &, std::ostream &, sogdanov::Datasets &);

  sogdanov::BSTree<std::string, cmd_t> commands;

  commands.push("print", sogdanov::cmdPrint);
  commands.push("complement", sogdanov::cmdComplement);
  commands.push("intersect", sogdanov::cmdIntersect);
  commands.push("union", sogdanov::cmdUnion);

  std::string cmd;
  while (std::cin >> cmd)
  {
    sogdanov::BSTIterator<std::string, cmd_t> cmd_it = commands.find(cmd);

    if (cmd_it != commands.end())
    {
      (*cmd_it).second(std::cin, std::cout, datasets);
    }
    else
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }
}
