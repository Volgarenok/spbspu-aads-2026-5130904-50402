#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <string>
#include "HashTable.hpp"
#include "hasher.hpp"
#include "vector.hpp"

namespace alisov
{
  class Graph
  {
  public:
    HashTable< std::string, bool, SHA1< std::string >, std::equal_to< std::string > > vertexes;
    HashTable< std::pair< std::string, std::string >, Vector< unsigned int >,
               SHA1< std::pair< std::string, std::string > >, std::equal_to< std::pair< std::string, std::string > > >
        edges;

    Graph():
      vertexes(16),
      edges(16)
    {}

    void addVertex(const std::string &name)
    {
      vertexes.insert(name, true);
    }

    void addEdge(const std::string &from, const std::string &to, unsigned int weight)
    {
      addVertex(from);
      addVertex(to);
      edges[std::make_pair(from, to)].push_back(weight);
    }
  };
}
#endif
