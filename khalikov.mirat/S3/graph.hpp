#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "../S1/list.hpp"
#include <functional>
#include <istream>
#include <ostream>
#include <string>
#include <utility>
#include "hashtable.hpp"
#include "hasher.hpp"

namespace khalikov
{
  using pairOfVertexes = std::pair< std::string, std::string >;
  struct Graph
  {
    HashTable< pairOfVertexes, List< size_t >, SipHash< pairOfVertexes >, std::equal_to< pairOfVertexes > > connections;
    List< std::string > vertexes;
    List< pairOfVertexes > pairs;
  };

  void swap(Graph &gr1, Graph &gr2) noexcept;

  using graphTable = HashTable< std::string, Graph, SipHash< std::string >, std::equal_to< std::string > >;

  graphTable parse(std::istream &in);

  template< class T >
  bool contains(const List< T > &list, const T &v);

  void graphs(std::ostream &out, std::istream &in, graphTable &table);
  void vertexes(std::ostream &out, std::istream &in, graphTable &table);
  void outbound(std::ostream &out, std::istream &in, graphTable &table);
  void inbound(std::ostream &out, std::istream &in, graphTable &table);
  void bind(std::ostream &out, std::istream &in, graphTable &table);
  void cut(std::ostream &out, std::istream &in, graphTable &table);
  void create(std::ostream &out, std::istream &in, graphTable &table);
  void merge(std::ostream &out, std::istream &in, graphTable &table);
  void extract(std::ostream &out, std::istream &in, graphTable &table);
};

#endif
