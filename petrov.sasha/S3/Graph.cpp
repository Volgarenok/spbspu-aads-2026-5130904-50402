#include "Graph.hpp"
#include <algorithm>
#include <set>

namespace petrov {
  Graph::Graph():
    edges_(100)
  {}

  void Graph::addEdge(const std::string& from, const std::string& to, unsigned int weight)
  {
    EdgeKey key = {from, to};
    if (edges_.has(key)) {
      EdgeValue& weights = edges_.get(key);
      weights.push_back(weight);
    } else {
      EdgeValue weights;
      weights.push_back(weight);
      edges_.add(key, weights);
    }
  }

  void Graph::removeEdge(const std::string& from, const std::string& to, unsigned int weight)
  {
    EdgeKey key = {from, to};
    if (!edges_.has(key)) {
      throw std::out_of_range("Edge not found");
    }
    EdgeValue& weights = edges_.get(key);
    auto it = std::find(weights.begin(), weights.end(), weight);
    if (it == weights.end()) {
      throw std::out_of_range("Weight not found");
    }
    weights.erase(it);
    if (weights.empty()) {
      edges_.drop(key);
    }
  }
}