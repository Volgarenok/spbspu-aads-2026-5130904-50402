#include <iostream>
#include <limits>

#include "commands.hpp"
#include "graph.hpp"

int main() {
  karpenkov::GraphManager manager;

  using Cmd =
      void (*)(std::istream &, std::ostream &, karpenkov::GraphManager &);

  using CommandTable =
      CuckooHashTable<std::string, Cmd, karpenkov::StringHash1,
                      karpenkov::StringHash2, karpenkov::StringEqual>;

  CommandTable commands{karpenkov::StringHash1{}, karpenkov::StringHash2{},
                        karpenkov::StringEqual{}};

  commands.add("help", karpenkov::cmdHelp);
  commands.add("create-order", karpenkov::cmdCreateOrder);
  commands.add("delete-order", karpenkov::cmdDeleteOrder);
  commands.add("insert-movie", karpenkov::cmdInsertMovie);
  commands.add("show-order", karpenkov::cmdShowOrder);
  commands.add("show-dependencies", karpenkov::cmdShowDependencies);
  commands.add("show-successors", karpenkov::cmdShowSuccessors);
  commands.add("find-cycles", karpenkov::cmdFindCycles);
  commands.add("list-orders", karpenkov::cmdListOrders);
  karpenkov::cmdHelp(std::cin, std::cout, manager);
  std::cout << '\n';

  std::string cmd;
  while (std::cin >> cmd) {
    try {
      commands.at(cmd)(std::cin, std::cout, manager);
    } catch (const std::exception &) {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }
}
