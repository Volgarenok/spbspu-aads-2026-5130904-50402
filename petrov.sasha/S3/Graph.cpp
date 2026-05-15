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

}