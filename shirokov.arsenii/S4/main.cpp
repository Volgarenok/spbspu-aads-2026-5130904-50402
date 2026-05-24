#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include "BSTree.hpp"

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    std::cerr << "Bad args\n";
    return 1;
  }

  std::ifstream in(argv[1]);
  if (!in.is_open())
  {
    std::cerr << "Couldn't open the file" << '\n';
    return 1;
  }

  using cmd_t = void (*)(std::ostream&, std::istream&);
  shirokov::BSTree< std::string, cmd_t, std::less< std::string > > cmds;

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      cmds.get(cmd)(std::cout, std::cin);
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>" << '\n';
      auto toIgnore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toIgnore, '\n');
    }
  }
  if (!std::cin.eof())
  {
    std::cerr << "Bad input" << '\n';
    return 1;
  }
}
