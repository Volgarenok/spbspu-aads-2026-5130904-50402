#include "GraphStorage.hpp"
#include "commands.hpp"
#include "HashTable.hpp"

#include <fstream>
#include <iostream>
#include <limits>
#include <string>
int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "Wrong arguments\n";
    return 1;
  }
  std::ifstream input(argv[1]);
  if (!input)
  {
    std::cerr << "Can't open file\n";
    return 1;
  }
  lachugin::GraphStorage storage;
  lachugin::loadGraphs(input, storage);
  using Cmd = void(*)(std::istream&, std::ostream&, lachugin::GraphStorage&);
  lachugin::HashTable< std::string, Cmd, std::hash< std::string >, std::equal_to< std::string > > commands(31, 4);
  commands.add("graphs", lachugin::cmdGraphs);
  commands.add("vertexes", lachugin::cmdVertexes);
  commands.add("outbound", lachugin::cmdOutbound);
  commands.add("inbound", lachugin::cmdInbound);
  commands.add("create", lachugin::cmdCreate);
  commands.add("bind", lachugin::cmdBind);
  commands.add("cut", lachugin::cmdCut);
  commands.add("merge", lachugin::cmdMerge);
  commands.add("extract", lachugin::cmdExtract);
  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      commands.get(cmd)(std::cin, std::cout, storage);
    }
    catch (const std::exception&)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }

}
