#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <bilist.hpp>
#include <functional>
#include <iostream>
#include <string>
#include <utility>
#include "HashTable.hpp"
#include "hasher.hpp"

namespace novikov
{
  using stringPair = std::pair< std::string, std::string >;
  struct Graph
  {
    HashTable< stringPair, List< size_t >, SipHash< stringPair >, std::equal_to< stringPair > > connections;
    List< std::string > vertexes;
    List< stringPair > pairs;
  };

  using graphTable = HashTable< std::string, Graph, SipHash< std::string >, std::equal_to< std::string > >;

  void graphs(std::ostream&, std::istream&, graphTable&, List< std::string >&);
  void vertexes(std::ostream&, std::istream&, graphTable&, List< std::string >&);
  void outbound(std::ostream&, std::istream&, graphTable&, List< std::string >&);
  void inbound(std::ostream&, std::istream&, graphTable&, List< std::string >&);
  void bind(std::ostream&, std::istream&, graphTable&, List< std::string >&);
  void cut(std::ostream&, std::istream&, graphTable&, List< std::string >&);
  void create(std::ostream&, std::istream&, graphTable&, List< std::string >&);
  void merge(std::ostream&, std::istream&, graphTable&, List< std::string >&);
  void extract(std::ostream&, std::istream&, graphTable&, List< std::string >&);
}

#endif
