#include <iostream>
#include <fstream>
#include <limits>
#include <string>
#include "commands.hpp"

int main(int argc, char *argv[])
{
  namespace muh = muhamadiarov;

  if (argc != 2)
  {
    std::cerr << "Incorrect usage\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file.is_open())
  {
    std::cerr << "Can't open file\n";
    return 1;
  }

  muh::GraphTable graphs(5, 5);

  std::string graphName;
  size_t edges = 0;
  try
  {
    while (file >> graphName >> edges)
    {
      muh::Graph g;
      for (size_t i = 0; i < edges; ++i)
      {
        std::string v1, v2;
        size_t w = 0;
        file >> v1 >> v2 >> w;
        g.addConnection(v1, v2, w);
      }
      graphs.add(graphName, std::move(g));
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << "Error loading graph: " << e.what() << '\n';
    return 1;
  }
  file.close();

  using cmd_t = void (*)(std::istream&, std::ostream&, muh::GraphTable&);
  using hash_t = muh::SipHash<std::string>;

  muh::HashTable<std::string, cmd_t, hash_t, std::equal_to<std::string> > commands(4, 8);

  commands.add("graphs", muh::cmdGraphs);
  commands.add("vertexes", muh::cmdVertexes);
  commands.add("outbound", muh::cmdOutbound);
  commands.add("inbound", muh::cmdInbound);
  commands.add("bind", muh::cmdBind);
  commands.add("cut", muh::cmdCut);
  commands.add("create", muh::cmdCreate);
  commands.add("merge", muh::cmdMerge);
  commands.add("extract", muh::cmdExtract);

  std::string cmd;
  while (std::cin >> cmd) {
    try
    {
      commands.get(cmd)(std::cin, std::cout, graphs);
    }
    catch (const std::exception &)
    {
      std::cout << "<INVALID COMMAND>" << '\n';
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  return 0;
}
