#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <limits>
#include "Graph.hpp"
#include "HashTable.hpp"
#include "SHA1Hash.hpp"

int main(int argc, char* argv[])
{
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file) {
    std::cerr << "Error opening file\n";
    return 1;
  }

  petrov::HashTable< std::string, petrov::Graph, petrov::SHA1Hasher< std::string >, petrov::DefaultEqual< std::string > > graphs(100);

  std::string line;
  while (std::getline(file, line)) {
    if (line.empty()) {
      continue;
    }

    std::istringstream iss(line);
    std::string graphName;
    size_t edgeCount = 0;
    iss >> graphName >> edgeCount;

    petrov::Graph graph;

    for (size_t i = 0; i < edgeCount; ++i) {
      std::string from;
      std::string to;
      unsigned int weight = 0;

      if (!(file >> from >> to >> weight)) {
        std::cerr << "Error reading edge data\n";
        return 1;
      }

      graph.addEdge(from, to, weight);
    }

    file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');

    graphs.add(graphName, graph);
  }

  file.close();

}