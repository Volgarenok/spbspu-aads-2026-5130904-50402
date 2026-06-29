#include "input.hpp"

#include <cstddef>
#include <istream>
#include <string>
#include "table_utils.hpp"

namespace samarin {

  static void readEdges(std::istream& in, std::size_t count, Graph& graph)
  {
    for (std::size_t i = 0; i < count; ++i) {
      std::string from;
      std::string to;
      Graph::Weight weight = 0;
      if (!(in >> from >> to >> weight)) {
        return;
      }
      graph.bind(from, to, weight);
    }
  }

}

void samarin::readGraphs(std::istream& in, GraphCollection& graphs)
{
  std::string name;
  while (in >> name) {
    std::size_t count = 0;
    if (!(in >> count)) {
      break;
    }
    Graph graph;
    readEdges(in, count, graph);
    insertOrGrow(graphs, name, graph);
  }
}
