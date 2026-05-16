#include <functional>
#include <iostream>
#include <istream>
#include <limits>
#include <ostream>
#include <string>
#include "HashTable.hpp"
#include "hasher.hpp"

int main()
{
  using cmd_t = void (*)(std::ostream&, std::istream&);
  shirokov::HashTable< std::string, cmd_t, shirokov::SHA1< std::string >, std::equal_to< std::string > > cmds;

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      cmds.at(cmd)(std::cout, std::cin);
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
      auto toIgnore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toIgnore, '\n');
    }
  }
  if (!std::cin.eof())
  {
    std::cerr << "Bad input\n";
    return 1;
  }
}
