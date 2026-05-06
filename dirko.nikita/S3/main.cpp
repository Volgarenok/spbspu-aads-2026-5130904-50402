#include <fstream>
#include <iostream>
#include "cmds.hpp"
int main(int argc, char *argv[])
{
  if (argc != 2) {
    std::cerr << "Incorrect usage";
    return 1;
  }
  std::ifstream file(argv[1]);
  if (!file.is_open()) {
    std::cerr << "Cant open file";
    return 1;
  }
  dirko::GraphTable graphs(20);
  std::string graph;
  size_t edges = 0;
  while (file >> graph >> edges) {
    dirko::Graph g;
    for (size_t i = 0; i < edges; ++i) {
      std::string v1, v2;
      size_t w = 0;
      file >> v1 >> v2 >> w;
      g.addConection(v1, v2, w);
    }
    graphs.add(graph, std::move(g));
  }
  file.close();

  using cmd_t = void (*)(std::istream &, std::ostream &, dirko::GraphTable &);
  using hash_t = dirko::SipHasher< std::string >;
  dirko::HashTable< std::string, cmd_t, hash_t, std::equal_to< std::string > > commands = {
      {"graphs", dirko::cmdGraphs},   {"vertexes", dirko::cmdConections}, {"outbound", dirko::cmdOutbound},
      {"inbound", dirko::cmdInbound}, {"bind", dirko::cmdBind},           {"cut", dirko::cmdCut},
      {"create", dirko::cmdCreate},   {"merge", dirko::cmdMerge},         {"extract", dirko::cmdExtract}};
  std::string cmd;
  while (std::cin >> cmd) {
    try {
      commands.get(cmd)(std::cin, std::cout, graphs);
    } catch (const std::exception &) {
      std::cout << "<INVALID COMMAND>" << '\n';
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}
