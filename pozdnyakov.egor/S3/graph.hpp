#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <utility>
#include "hashTable.hpp"
#include "sha1.hpp"
#include "vector.hpp"

namespace pozdnyakov
{
  struct EdgeInfo
  {
    std::string from;
    std::string to;
    unsigned int weight;
  };

  struct NodeEdges
  {
    std::string vertex;
    Vector< unsigned int > weights;
  };

  class Graph
  {
  private:
    Vector< std::string > vertices_;
    HashTable< std::pair< std::string, std::string >, Vector< unsigned int >, hash::GraphEdgeSHA1 > edges_;

  public:
    Graph() = default;

    void addVertex(const std::string &v);

    bool hasVertex(const std::string &v) const;

    void addEdge(const std::string &from, const std::string &to, unsigned int weight);

    bool removeEdge(const std::string &from, const std::string &to, unsigned int weight);

    const Vector< std::string > &getVertices() const;

    Vector< NodeEdges > getOutboundEdges(const std::string &from);

    Vector< NodeEdges > getInboundEdges(const std::string &to);

    Vector< EdgeInfo > getAllEdges();
  };
}

#endif
