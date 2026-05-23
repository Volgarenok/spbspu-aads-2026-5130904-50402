#include <bilist.hpp>
#include <fstream>
#include <functional>
#include <iostream>
#include <istream>
#include <limits>
#include <ostream>
#include <string>
#include "Graph.hpp"
#include "HashTable.hpp"
#include "functions.hpp"
#include "hasher.hpp"

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
    std::cerr << "Couldn't open the file\n";
    return 1;
  }
  std::pair< shirokov::graphTable, shirokov::BiList< std::string > > parseRes = shirokov::parse(in);
  shirokov::graphTable graphs = parseRes.first;
  shirokov::BiList< std::string > names = parseRes.second;

  using cmd_t = void (*)(std::ostream&, std::istream&, shirokov::graphTable&, shirokov::BiList< std::string >&);
  shirokov::HashTable< std::string, cmd_t, shirokov::SHA1< std::string >, std::equal_to< std::string > > cmds;
  cmds["graphs"] = shirokov::graphs;
  cmds["vertexes"] = shirokov::vertexes;
  cmds["outbound"] = shirokov::outbound;
  cmds["inbound"] = shirokov::inbound;
  cmds["bind"] = shirokov::bind;
  cmds["cut"] = shirokov::cut;
  cmds["create"] = shirokov::create;
  // cmds["merge"] = shirokov::merge;
  // cmds["extract"] = shirokov::extract;

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
    std::cerr << "Bad input\n";
    return 1;
  }
}
