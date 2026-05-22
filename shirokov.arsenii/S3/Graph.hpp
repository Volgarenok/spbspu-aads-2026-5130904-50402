#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <bilist.hpp>
#include <functional>
#include <istream>
#include <ostream>
#include <string>
#include <utility>
#include "HashTable.hpp"
#include "hasher.hpp"

namespace shirokov
{
  using pairOfVertexes = std::pair< std::string, std::string >;
  struct Graph
  {
    HashTable< pairOfVertexes, BiList< size_t >, SHA1< pairOfVertexes >, std::equal_to< pairOfVertexes > > connections;
    BiList< std::string > vertexes;
    BiList< pairOfVertexes > pairs;
  };

  void swap(Graph& gr1, Graph& gr2);

  using graphTable = HashTable< std::string, Graph, SHA1< std::string >, std::equal_to< std::string > >;

  void graphs(std::ostream&, std::istream&, graphTable&, BiList< std::string >&);
  void vertexes(std::ostream&, std::istream&, graphTable&, BiList< std::string >&);
  void outbound(std::ostream&, std::istream&, graphTable&, BiList< std::string >&);
  void inbound(std::ostream&, std::istream&, graphTable&, BiList< std::string >&);
  void bind(std::ostream&, std::istream&, graphTable&, BiList< std::string >&);
  void cut(std::ostream&, std::istream&, graphTable&, BiList< std::string >&);
  void create(std::ostream&, std::istream&, graphTable&, BiList< std::string >&);
  void merge(std::ostream&, std::istream&, graphTable&, BiList< std::string >&);
  void extract(std::ostream&, std::istream&, graphTable&, BiList< std::string >&);
}

#endif
