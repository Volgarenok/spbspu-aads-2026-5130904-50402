#ifndef GRAPH_HPP
#define GRAPH_HPP
#include "../common/list.hpp"
#include "../common/itters.hpp"
#include "HashTable.hpp"
#include "Edge.hpp"

namespace lachugin
{
  class Graph
  {
  public:
    using Weights = List< size_t >;
    Graph();

    void addVertex(const std::string& name);
    bool hasVertex(const std::string& name) const;
    const List< std::string >& getVertexes() const;
    const HashTable< Edge, Weights, EdgeHash, EdgeEqual >& getEdges() const;
    size_t vertexesCount() const;

    void bind(const std::string& from, const std::string& to, size_t weight);
    bool hasEdge(const std::string& from, const std::string& to) const;
    void cut(const std::string& from, const std::string& to, size_t weight);

    List< Edge > outbound(const std::string& vertex) const;
    List< Edge > inbound(const std::string& vertex) const;
  private:
    List< std::string > vertexes_;

    HashTable< Edge, Weights, EdgeHash, EdgeEqual > edges_;
  };


}


#endif
