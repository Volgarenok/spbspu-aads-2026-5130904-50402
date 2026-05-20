#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <bilist.hpp>
#include <functional>
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
  };

  using graphTable = HashTable< std::string, Graph&, SHA1< std::string >, std::equal_to< std::string > >;

  void graphs(std::ostream&, std::istream&, graphTable&);
  void vertexes(std::ostream&, std::istream&, graphTable&);
  void outbound(std::ostream&, std::istream&, graphTable&);
  void inbound(std::ostream&, std::istream&, graphTable&);
  void bind(std::ostream&, std::istream&, graphTable&);
  void cut(std::ostream&, std::istream&, graphTable&);
  void create(std::ostream&, std::istream&, graphTable&);
  void merge(std::ostream&, std::istream&, graphTable&);
  void extract(std::ostream&, std::istream&, graphTable&);
}

#endif
