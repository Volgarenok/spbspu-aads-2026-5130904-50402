#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <vector>
#include <utility>
#include "HashTable.hpp"
#include "SHA1Hash.hpp"

namespace petrov {

  class Graph {
  public:
    Graph();

    void addEdge(const std::string& from, const std::string& to, unsigned int weight);
    void removeEdge(const std::string& from, const std::string& to, unsigned int weight);
    std::vector< std::string > getVertices() const;
    std::vector< std::pair< std::string, std::vector< unsigned int > > > getOutbound(const std::string& vertex) const;
    std::vector< std::pair< std::string, std::vector< unsigned int > > > getInbound(const std::string& vertex) const;

  private:
    using EdgeKey = std::pair< std::string, std::string >;
    using EdgeValue = std::vector< unsigned int >;
    HashTable< EdgeKey, EdgeValue, SHA1Hasher< EdgeKey >, DefaultEqual< EdgeKey > > edges_;
  };
}

#endif

