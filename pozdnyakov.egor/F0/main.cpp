#include <iostream>
#include <limits>
#include <new>
#include <stdexcept>
#include <string>
#include "commands.hpp"

int main(int argc, char **argv)
{
  if (argc > 1) {
    std::cerr << "<INVALID ARGUMENTS: " << argv[0] << " takes no parameters>\n";
    return 1;
  }

  pozdnyakov::DictTable dicts;
  pozdnyakov::CmdTable cmds;

  try {
    pozdnyakov::initCommands(cmds);
  } catch (const std::exception &e) {
    std::cerr << e.what() << "\n";
    return 2;
  }

  std::string cmdName;
  while (std::cin >> cmdName) {
    try {
      pozdnyakov::CommandFunc * const command = cmds.find(cmdName);
      if (command == nullptr) {
        throw std::invalid_argument("<INVALID COMMAND>");
      }
      (*command)(dicts, std::cin, std::cout);
    } catch (const std::bad_alloc &) {
      std::cerr << "<OUT OF MEMORY>\n";
      return 2;
    } catch (const std::exception &e) {
      std::cout << e.what() << "\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  return 0;
}
