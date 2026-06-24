#include <iostream>
#include <fstream>
#include <limits>
#include "bstree.hpp"
#include "commands.hpp"

int main(int argc, char** argv)
{
  namespace muh = muhamadiarov;

  if (argc != 2)
  {
    std::cerr << "Incomplete argument\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file.is_open())
  {
    std::cerr << "Cannot open file\n";
    return 1;
  }

  muh::ManyDatasets_t ds;
  std::string word;
  std::string dataset;
  while (file >> word)
  {
    if (muh::isInt(word))
    {
      int number = std::stoi(word);
      std::string data;
      if (file >> data)
      {
        ds.get(dataset).push(number, data);
      }
    }
    else
    {
      if (ds.has(word))
      {
        dataset = word;
      }
      else
      {
        ds.push(word, muh::BSTree< int, std::string >());
        dataset = word;
      }
    }
  }
  file.close();

  using Command_t = void (*)(std::istream&, std::ostream&, muh::ManyDatasets_t&);
  muh::BSTree< std::string, Command_t > commands;
  commands.push("print", muh::cmdPrint);
  commands.push("complement", muh::cmdComplement);
  commands.push("intersect", muh::cmdIntersect);
  commands.push("union", muh::cmdUnion);

  std::string cmd;
  while (std::cin >> cmd) {
    try {
      commands.get(cmd)(std::cin, std::cout, ds);
    } catch (const std::exception &) {
      std::cout << "<INVALID COMMAND>" << '\n';
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  return 0;
}
