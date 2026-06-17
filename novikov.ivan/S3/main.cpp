#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <string>
#include <bilist.hpp>
#include "Graph.hpp"
#include "HashTable.hpp"
#include "hasher.hpp"
#include "Parser.hpp"

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    std::cerr << "Invalid arguments\n";
    return 1;
  }

  std::ifstream in(argv[1]);
  if (!in.is_open())
  {
    std::cerr << "Could not open the file\n";
    return 1;
  }
  std::pair< novikov::graphTable, novikov::List< std::string > > parseRes = novikov::parse(in);
  novikov::graphTable graphs = parseRes.first;
  novikov::List< std::string > names = parseRes.second;

  using cmd_t = void (*)(std::ostream&, std::istream&, novikov::graphTable&, novikov::List< std::string >&);
  novikov::HashTable< std::string, cmd_t, novikov::SipHash< std::string >, std::equal_to< std::string > > cmds;
  cmds["graphs"] = novikov::graphs;
  cmds["vertexes"] = novikov::vertexes;
  cmds["outbound"] = novikov::outbound;
  cmds["inbound"] = novikov::inbound;
  cmds["bind"] = novikov::bind;
  cmds["cut"] = novikov::cut;
  cmds["create"] = novikov::create;
  cmds["merge"] = novikov::merge;
  cmds["extract"] = novikov::extract;

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      cmds.at(cmd)(std::cout, std::cin, graphs, names);
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
    std::cerr << "Invalid input\n";
    return 1;
  }
}
