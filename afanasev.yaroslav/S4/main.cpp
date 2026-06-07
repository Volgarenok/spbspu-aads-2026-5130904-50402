#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <stdexcept>
#include "BiTree/BiTree.hpp"
#include "commands.hpp"

namespace afanasev
{
  bool isInt(const std::string & str)
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

    if (start == str.size())
    {
      return false;
    }

    for (size_t i = start; i < str.size(); ++i)
    {
      if (str[i] < '0' || str[i] > '9')
      {
        return false;
      }
    }
    return true;
  }
}

int main(int argc, char * argv[])
{
  if (argc != 2)
  {
    std::cerr << "need two arg";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file.is_open())
  {
    std::cerr << "Skill Issue netu faila: " << argv[1] << '\n';
    return 1;
  }

  afanasev::Datasets ds;
  std::string word;
  std::string currentDataset;

  while (file >> word)
  {
    if (afanasev::isInt(word))
    {
      int key = std::stoi(word);
      std::string value;

      if (file >> value)
      {
        ds.get(currentDataset).push(key, value);
      }
    }
    else
    {
      currentDataset = word;
      try
      {
        ds.get(currentDataset);
      }
      catch (const std::out_of_range &)
      {
        ds.push(currentDataset, afanasev::Dataset{});
      }
    }
  }

  file.close();

  using CmdFunc = void (*)(std::istream &, std::ostream &, afanasev::Datasets &);
  afanasev::BSTree< std::string, CmdFunc > commands;

  commands.push("print",      afanasev::cmdPrint);
  commands.push("complement", afanasev::cmdComplement);
  commands.push("intersect",  afanasev::cmdIntersect);
  commands.push("union",      afanasev::cmdUnion);

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      commands.get(cmd)(std::cin, std::cout, ds);
    }
    catch (const std::exception &)
    {
      std::cout << "<INVALID COMMAND>" << '\n';
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  return 0;
}
