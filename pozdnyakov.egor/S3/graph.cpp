#include "graph.hpp"

namespace pozdnyakov
{
  void Graph::addVertex(const std::string &v)
  {
    for (std::size_t i = 0; i < vertices_.size(); ++i) {
      if (vertices_[i] == v) {
        return;
      }
    }
    vertices_.pushBack(v);
  }

  bool Graph::hasVertex(const std::string &v) const
  {
    for (std::size_t i = 0; i < vertices_.size(); ++i) {
      if (vertices_[i] == v) {
        return true;
      }
    }
    return false;
  }

  void Graph::addEdge(const std::string &from, const std::string &to, unsigned int weight)
  {
    addVertex(from);
    addVertex(to);

    std::pair< std::string, std::string > key{from, to};

    if (edges_.has(key)) {
      edges_.at(key).pushBack(weight);
    } else {
      Vector< unsigned int > weights;
      weights.pushBack(weight);
      edges_.add(key, std::move(weights));
    }
  }

  bool Graph::removeEdge(const std::string &from, const std::string &to, unsigned int weight)
  {
    std::pair< std::string, std::string > key{from, to};
    if (edges_.has(key)) {
      Vector< unsigned int > &weights = edges_.at(key);
      for (std::size_t i = 0; i < weights.size(); ++i) {
        if (weights[i] == weight) {
          for (std::size_t j = i + 1; j < weights.size(); ++j) {
            weights[j - 1] = std::move(weights[j]);
          }
          weights.resize(weights.size() - 1);
          if (weights.size() == 0) {
            edges_.drop(key);
          }
          return true;
        }
      }
    }
    return false;
  }

  const Vector< std::string > &Graph::getVertices() const
  {
    return vertices_;
  }

  Vector< NodeEdges > Graph::getOutboundEdges(const std::string &from)
  {
    Vector< NodeEdges > result;
    for (auto it = edges_.begin(); it != edges_.end(); ++it) {
      if ((*it).first.first == from) {
        result.pushBack({(*it).first.second, (*it).second});
      }
    }
    return result;
  }

  Vector< NodeEdges > Graph::getInboundEdges(const std::string &to)
  {
    Vector< NodeEdges > result;
    for (auto it = edges_.begin(); it != edges_.end(); ++it) {
      if ((*it).first.second == to) {
        result.pushBack({(*it).first.first, (*it).second});
      }
    }
    return result;
  }

  Vector< EdgeInfo > Graph::getAllEdges()
  {
    Vector< EdgeInfo > result;
    for (auto it = edges_.begin(); it != edges_.end(); ++it) {
      const std::string &fromNode = (*it).first.first;
      const std::string &toNode = (*it).first.second;
      const Vector< unsigned int > &weights = (*it).second;
      for (std::size_t i = 0; i < weights.size(); ++i) {
        result.pushBack({fromNode, toNode, weights[i]});
      }
    }
    return result;
  }
}
