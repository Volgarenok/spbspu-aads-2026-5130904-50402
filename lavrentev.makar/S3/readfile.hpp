#ifndef READFILE_HPP
#define READFILE_HPP

#include "Graph.hpp"
#include <fstream>
#include <stdexcept>

namespace lavrentev {
  void readfile(std::string name, List<std::pair<std::string, Graph>> &grs);
}

inline void lavrentev::readfile(std::string name, List<std::pair<std::string, Graph>> &grs)
{
  std::ifstream file(name);
  if (file.is_open())
  {
    std::string graph;
    size_t edges;
    while (file >> graph >> edges)
    {
      createWithArg(graph, grs);
      for(size_t i = 0; i < edges; ++i)
      {
        std::string v1, v2;
        size_t weight;

        if (!(file >> v1 >> v2 >> weight))
        {
          throw std::runtime_error("Invalid file format");
        }

        bindWithArg(graph, v1, v2, weight, grs);
      }
    }
    file.close();
  }
  else
  {
    throw std::runtime_error("File open error");
  }
}

#endif
