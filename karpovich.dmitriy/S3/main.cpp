#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include "commands.hpp"
#include "graph.hpp"
#include "hashFunctions.hpp"
#include "hashTable.hpp"

int main(int argc, char *argv[])
{
  if (argc != 2) {
    std::cerr << "bad num of args" << '\n';
    return 1;
  }
  karpovich::GraphSet graphs(16);
  std::ifstream file(argv[1]);
  if (!file.is_open()) {
    std::cerr << "Cannot open file" << '\n';
    return 1;
  }

  std::string graph_name;
  size_t edge_count = 0;
  while (file >> graph_name >> edge_count) {
    karpovich::Graph g;
    for (size_t i = 0; i < edge_count; ++i) {
      std::string v1, v2;
      size_t w = 0;
      file >> v1 >> v2 >> w;
      g.addEdge(v1, v2, w);
    }
    graphs.add(graph_name, std::move(g));
  }
  file.close();

  using cmd_t = void (*)(std::istream &, std::ostream &, karpovich::GraphSet &);
  using hash_t = karpovich::Hasher< std::string >;
  karpovich::HashTable< std::string, cmd_t, hash_t, std::equal_to< std::string > > commands(16);

  commands.add("graphs", karpovich::cmdGraphs);
  commands.add("vertexes", karpovich::cmdVertexes);
  commands.add("outbound", karpovich::cmdOutbound);
  commands.add("inbound", karpovich::cmdInbound);
  commands.add("bind", karpovich::cmdBind);
  commands.add("cut", karpovich::cmdCut);
  commands.add("create", karpovich::cmdCreate);
  commands.add("merge", karpovich::cmdMerge);
  commands.add("extract", karpovich::cmdExtract);

  std::string cmd;
  while (std::cin >> cmd) {
    try {
      if (commands.has(cmd)) {
        commands.get(cmd)(std::cin, std::cout, graphs);
      } else {
        throw std::runtime_error("Unknown");
      }
    } catch (const std::exception &) {
      std::cout << "<INVALID COMMAND>" << '\n';
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  return 0;
}
