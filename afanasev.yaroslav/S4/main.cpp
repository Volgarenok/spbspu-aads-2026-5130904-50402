#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <stdexcept>
#include "BiTree/BiTree.hpp"
#include "commands.hpp"

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

  file.close();


  using CmdFunc = void (*)(std::istream &, std::ostream &, afanasev::Datasets &);
  afanasev::BSTree< std::string, CmdFunc > commands;

  commands.push("print",      afanasev::cmdPrint);
  commands.push("complement", afanasev::cmdComplement);
  commands.push("intersect",  afanasev::cmdIntersect);
  commands.push("union",      afanasev::cmdUnion);

  /*
  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      commands.get(cmd)(std::cin, std::cout, graphs);
    }
    catch (const std::exception &)
    {
      std::cout << "<INVALID COMMAND>" << '\n';
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }*/

  return 0;
}
