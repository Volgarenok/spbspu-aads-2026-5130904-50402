#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <stdexcept>
//#include "HashTable/HashTable.hpp"
//#include "HashTable/HashFunction.hpp"
//#include "Graph.hpp"
#include "commands.hpp"

int main(int argc, char * argv[])
{
  if (argc != 2)
  {
    std::cerr << "need two arg";
    return 1;
  }








  
  std::ifstream file(argv[1]);
  if (!file.is_open())
  {
    std::cerr << "Skill Issue netu faila: " << argv[1] << '\n';
    return 1;
  }

  file.close();

  /*
  using CmdFunc = void (*)(std::istream &, std::ostream &, afanasev::GraphSet &);
  using CmdHash = afanasev::Hasher< std::string >;
  afanasev::HashTable< std::string, CmdFunc, CmdHash, std::equal_to< std::string > > commands(9);

  commands.add("graphs",   afanasev::cmdGraphs);
  commands.add("vertexes", afanasev::cmdVertexes);
  commands.add("outbound", afanasev::cmdOutbound);
  commands.add("inbound",  afanasev::cmdInbound);
  commands.add("bind",     afanasev::cmdBind);
  commands.add("cut",      afanasev::cmdCut);
  commands.add("create",   afanasev::cmdCreate);
  commands.add("merge",    afanasev::cmdMerge);
  commands.add("extract",  afanasev::cmdExtract);

  std::string cmd;
  while (std::cin >> cmd)
  {
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
*/
  return 0;
}
