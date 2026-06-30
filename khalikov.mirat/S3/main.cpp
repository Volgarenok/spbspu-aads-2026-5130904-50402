#include <fstream>
#include <functional>
#include <iostream>
#include <istream>
#include <limits>
#include <ostream>
#include <string>
#include "../S1/list.hpp"
#include "graph.hpp"
#include "hashtable.hpp"
#include "hasher.hpp"

int main(int argc, char *argv[])
{
  if (argc < 2) {
    std::cerr << "<INVALID COMMAND>\n";
    return 1;
  }
  std::ifstream in(argv[1]);
  if (!in.is_open()) {
    std::cerr << "Couldn't open the file\n";
    return 1;
  }
  khalikov::graphTable table = khalikov::parse(in);
  in.close();
  using cmd_t = void (*)(std::ostream &, std::istream &, khalikov::graphTable &);
  khalikov::HashTable< std::string, cmd_t, khalikov::SipHash< std::string >, std::equal_to< std::string > > cmds;
  cmds["graphs"] = khalikov::graphs;
  cmds["vertexes"] = khalikov::vertexes;
  cmds["outbound"] = khalikov::outbound;
  cmds["inbound"] = khalikov::inbound;
  cmds["bind"] = khalikov::bind;
  cmds["cut"] = khalikov::cut;
  cmds["create"] = khalikov::create;
  cmds["merge"] = khalikov::merge;
  cmds["extract"] = khalikov::extract;
  std::string cmd;
  while (std::cin >> cmd) {
    try {
      cmds.at(cmd)(std::cout, std::cin, table);
    } catch (...) {
      std::cout << "<INVALID COMMAND>\n";
      auto toignore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toignore, '\n');
    }
  }
  if (!std::cin.eof()) {
    std::cerr << "Bad input\n";
    return 1;
  }
}
