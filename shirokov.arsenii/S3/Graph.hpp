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

  void graphs(std::ostream&, std::istream&, Graph&);
  void vertexes(std::ostream&, std::istream&, Graph&);
  void outbound(std::ostream&, std::istream&, Graph&);
  void inbound(std::ostream&, std::istream&, Graph&);
  void bind(std::ostream&, std::istream&, Graph&);
  void cut(std::ostream&, std::istream&, Graph&);
  void create(std::ostream&, std::istream&, Graph&);
  void merge(std::ostream&, std::istream&, Graph&);
  void extract(std::ostream&, std::istream&, Graph&);
}

#endif
