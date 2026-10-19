#include <iostream>
#include <limits>
#include "operations.hpp"

int main()
{
  using cmd_t = void(*)(std::istream &, permyakov::Manager &);
  permyakov::RobinTable< std::string, cmd_t > cmds(11);
  
  cmds.add("bin", permyakov::bin);
  cmds.add("sum", permyakov::sum);
  cmds.add("multi", permyakov::multi);
  cmds.add("make", permyakov::make);
  cmds.add("construct", permyakov::construct);
  cmds.add("copy", permyakov::copy);
  cmds.add("reverse", permyakov::reverse);
  cmds.add("get", permyakov::get);
  cmds.add("bayes", permyakov::bayes);
  cmds.add("deleteS", permyakov::deleteS);
  cmds.add("deleteE", permyakov::deleteE);

  spc_t table{};
  std::string filename = "spaces";
  permyakov::Manager manager(filename, table, std::cout);

  std::string cmd;
  while (std::cin >> cmd) {
    try {
      if (std::cin.fail()) {
        std::cin.clear(std::cin.rdstate() & ~std::ios::failbit);
        std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      }
      manager.loadFile();
      cmds.get(cmd)(std::cin, manager);
    } catch (const std::exception &e) {
      std::cout << e.what() << '\n';
      std::cin.clear(std::cin.rdstate() & ~std::ios::failbit);
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}
