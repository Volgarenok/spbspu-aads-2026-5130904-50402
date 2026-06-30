#include <iostream>
#include "HashTable.hpp"
#include "commands.hpp"
#include "graph.hpp"
#include "parser.hpp"

int main(int argc, char *argv[])
{
  if (argc < 2) {
    std::cerr << "Error: No file\n";
    return 1;
  }
  alisov::GraphTable graphs(16);
  if (!alisov::loadFromFile(argv[1], graphs)) {
    std::cerr << "Error\n";
    return 1;
  }

  std::string cmd;
  while (std::cin >> cmd) {
    if (cmd == "graphs") {
      alisov::handleGraphs(graphs);
    } else if (cmd == "vertexes") {
      std::string g;
      std::cin >> g;
      alisov::handleVertexes(graphs, g);
    } else if (cmd == "bind") {
      std::string g, f, t;
      unsigned int w;
      std::cin >> g >> f >> t >> w;
      alisov::handleBind(graphs, g, f, t, w);
    } else if (cmd == "merge") {
      std::string r, g1, g2;
      std::cin >> r >> g1 >> g2;
      alisov::handleMerge(graphs, r, g1, g2);
    } else {
      std::cout << "Invalid command\n";
    }
  }
}
