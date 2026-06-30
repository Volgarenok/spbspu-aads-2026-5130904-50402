#ifndef PARSER_HPP
#define PARSER_HPP
#include <fstream>
#include <string>
#include "HashTable.hpp"
#include "graph.hpp"

namespace alisov
{
  inline bool loadFromFile(const std::string &filename,
                           HashTable< std::string, Graph, SHA1< std::string >, std::equal_to< std::string > > &graphs)
  {
    std::ifstream file(filename);
    if (!file.is_open()) {
      return false;
    }
    std::string line;
    while (file >> line) {
      std::string gname = line;
      size_t edge_count;
      if (!(file >> edge_count)) {
        break;
      }
      Graph g;
      for (size_t i = 0; i < edge_count; ++i) {
        std::string from, to;
        unsigned int w;
        if (file >> from >> to >> w) {
          g.addEdge(from, to, w);
        }
      }
      graphs.insert(gname, g);
    }
    return true;
  }
}
#endif
