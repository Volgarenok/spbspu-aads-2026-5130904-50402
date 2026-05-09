#include "graphReader.hpp"

#include <istream>
#include <stdexcept>
#include <string>

namespace matveev
{
void readGraphs(std::istream& input, GraphCollection& graphs)
{
  std::string graph_name;

  while (input >> graph_name)
  {
    size_t edge_count = 0;

    if (!(input >> edge_count))
    {
      throw std::logic_error("invalid graph header");
    }

    Graph graph;

    for (size_t i = 0; i < edge_count; ++i)
    {
      std::string from;
      std::string to;
      unsigned long long weight = 0;

      if (!(input >> from >> to >> weight))
      {
        throw std::logic_error("invalid edge");
      }

      graph.bind(from, to, weight);
    }

    graphs.addGraph(graph_name, graph);
  }
}
}
