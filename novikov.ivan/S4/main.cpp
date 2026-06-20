#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <bilist.hpp>
#include "BSTree.hpp"
#include "Parse.hpp"
#include "commands.hpp"

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    std::cerr << "Invalid arguments\n";
    return 1;
  }

  novikov::List< novikov::BSTree< size_t, std::string, std::less< size_t > > > trees;

  try
  {
    std::ifstream file(argv[1]);
    if (!file.is_open())
    {
      throw std::runtime_error("Could not open the file");
    }
    novikov::parse(file, trees);
  }
  catch (const std::runtime_error&)
  {
    std::cerr << "Input processing error" << "\n";
    return 2;
  }

  using cmd_t = void (*)(std::istream& in, std::ostream& out, novikov::BSTList& bstl);
  novikov::BSTree< std::string, cmd_t, std::less< std::string > > cmds;
  cmds["print"] = novikov::print;
  cmds["complement"] = novikov::complement;
  cmds["intersect"] = novikov::intersect;
  cmds["union"] = novikov::union_;

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      if (!cmds.has(cmd))
      {
        throw std::logic_error("Invalid command");
      }
      cmds[cmd](std::cin, std::cout, trees);
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>" << '\n';
      std::cin.clear();
      auto toIgnore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toIgnore, '\n');
    }
  }
  if (!std::cin.eof())
  {
    std::cerr << "Invalid input\n";
    return 1;
  }
}
