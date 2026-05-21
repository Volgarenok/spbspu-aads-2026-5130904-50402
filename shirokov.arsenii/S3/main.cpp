#include <fstream>
#include <functional>
#include <iostream>
#include <istream>
#include <limits>
#include <ostream>
#include <string>
#include "Graph.hpp"
#include "HashTable.hpp"
#include "bilist.hpp"
#include "hasher.hpp"

namespace shirokov
{
  shirokov::graphTable parse(std::istream& in);

  template< class T >
  bool has(const BiList< T >& list, const T& elem);
}

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
  shirokov::graphTable graphs = shirokov::parse(in);

  using cmd_t = void (*)(std::ostream&, std::istream&, shirokov::graphTable&);
  shirokov::HashTable< std::string, cmd_t, shirokov::SHA1< std::string >, std::equal_to< std::string > > cmds;
  cmds["graphs"] = shirokov::graphs;
  cmds["vertexes"] = shirokov::vertexes;
  cmds["outbound"] = shirokov::outbound;
  cmds["inbound"] = shirokov::inbound;
  cmds["bind"] = shirokov::bind;
  cmds["cut"] = shirokov::cut;
  cmds["create"] = shirokov::create;
  cmds["merge"] = shirokov::merge;
  cmds["extract"] = shirokov::extract;

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      cmds.at(cmd)(std::cout, std::cin, graphs);
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

shirokov::graphTable shirokov::parse(std::istream& in)
{
  shirokov::graphTable gt;
  while (in)
  {
    std::string name;
    size_t count;
    in >> name >> count;
    shirokov::Graph graph;
    for (size_t i = 0; i < count; ++i)
    {
      std::string v1, v2;
      size_t weight;
      in >> v1 >> v2 >> weight;
      graph.connections[{v1, v2}].pushBack(weight);
      if (!has(graph.vertexes, v1))
      {
        graph.vertexes.pushBack(v1);
      }
      if (!has(graph.vertexes, v2))
      {
        graph.vertexes.pushBack(v2);
      }
    }
    gt.insert(name, &graph);
  }
  return gt;
}
