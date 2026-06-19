#include "Graph.hpp"
#include "HashTable.hpp"

namespace lachugin {

  Graph::Graph():
    edges_(50, 5, 20)
  {}

  bool Graph::hasEdge(const std::string &from, const std::string &to) const
  {
    Edge edge{from, to};
    return edges_.has(edge);
  }

  void Graph::bind(const std::string& from, const std::string& to, size_t weight)
  {
    addVertex(from);
    addVertex(to);
    Edge edge;
    edge.from = from;
    edge.to = to;
    if (!edges_.has(edge))
    {
      Weights weights;
      weights.pushBack(weight);
      edges_.add(edge, weights);
      return;
    }
    edges_.get(edge).pushBack(weight);
  }

  void Graph::cut(const std::string& from,const std::string& to, size_t weight)
  {
    Edge edge;
    edge.from = from;
    edge.to = to;
    if (!hasVertex(from) || !hasVertex(to))
    {
      throw std::logic_error("Vertex not found");
    }

    if (!edges_.has(edge))
    {
      throw std::logic_error("Edge not found");
    }
    Weights& weights = edges_.get(edge);
    for (auto it = weights.begin(); it != weights.end(); ++it)
    {
      if (*it == weight)
      {
        weights.erase(it);
        if (weights.empty())
        {
          edges_.drop(edge);
        }
        return;
      }
    }
    throw std::logic_error("Weight not found");
  }

  bool Graph::hasVertex(const std::string &name) const
  {
    for (auto it = vertexes_.begin(); it != vertexes_.end(); ++it)
    {
    if (*it == name)
      {
      return true;
      }
    }
    return false;
  }

  void Graph::addVertex(const std::string& name)
  {
    if (!hasVertex(name))
    {
      vertexes_.pushBack(name);
    }
  }

  const List< std::string >& Graph::getVertexes() const
  {
    return vertexes_;
  }

  size_t Graph::vertexesCount() const
  {
    return vertexes_.size();
  }

  List< Edge > Graph::outbound(const std::string& vertex) const
  {
    List< Edge > result;
    for (auto it = edges_.begin(); it != edges_.end(); ++it)
    {
      if (it->key.from == vertex)
      {
        result.pushBack(it->key);
      }
    }

    return result;
  }

  List< Edge > Graph::inbound(const std::string& vertex) const
  {
    List< Edge > result;
    for (auto it = edges_.begin(); it != edges_.end(); ++it)
    {
      if (it->key.to == vertex)
      {
        result.pushBack(it->key);
      }
    }

    return result;
  }

  const HashTable<Edge, Graph::Weights, EdgeHash, EdgeEqual>& Graph::getEdges() const
  {
    return edges_;
  }

}
