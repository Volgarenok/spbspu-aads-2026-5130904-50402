#include "Graph.hpp"
#include <algorithm>
#include <set>

namespace petrov {

  Graph::Graph():
    edges_(100),
    vertices_()
  {}

  void Graph::addEdge(const std::string& from, const std::string& to, unsigned int weight)
  {
    vertices_.insert(from);
    vertices_.insert(to);
    
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

  std::vector< std::string > Graph::getVertices() const
  {
    return std::vector< std::string >(vertices_.begin(), vertices_.end());
  }

  std::vector< std::pair< std::string, std::vector< unsigned int > > > Graph::getOutbound(const std::string& vertex) const
  {
    HashTable< std::string, EdgeValue, SHA1Hasher< std::string >, DefaultEqual< std::string > > result(50);

    for (auto it = edges_.cbegin(); it != edges_.cend(); ++it) {
      auto edge = *it;
      if (edge.first.first == vertex) {
        const std::string& to = edge.first.second;
        const EdgeValue& weights = edge.second;

        if (result.has(to)) {
          EdgeValue& existingWeights = result.get(to);
          existingWeights.insert(existingWeights.end(), weights.begin(), weights.end());
        } else {
          result.add(to, weights);
        }
      }
    }

    std::vector< std::pair< std::string, EdgeValue > > output;
    for (auto it = result.begin(); it != result.end(); ++it) {
      auto pair = *it;
      EdgeValue sortedWeights = pair.second;
      std::sort(sortedWeights.begin(), sortedWeights.end());
      output.push_back({pair.first, sortedWeights});
    }

    std::sort(output.begin(), output.end(), [](const auto& a, const auto& b) {
      return a.first < b.first;
    });

    return output;
  }

  std::vector< std::pair< std::string, std::vector< unsigned int > > > Graph::getInbound(const std::string& vertex) const
  {
    HashTable< std::string, EdgeValue, SHA1Hasher< std::string >, DefaultEqual< std::string > > result(50);

    for (auto it = edges_.cbegin(); it != edges_.cend(); ++it) {
      auto edge = *it;
      if (edge.first.second == vertex) {
        const std::string& from = edge.first.first;
        const EdgeValue& weights = edge.second;

        if (result.has(from)) {
          EdgeValue& existingWeights = result.get(from);
          existingWeights.insert(existingWeights.end(), weights.begin(), weights.end());
        } else {
          result.add(from, weights);
        }
      }
    }

    std::vector< std::pair< std::string, EdgeValue > > output;
    for (auto it = result.begin(); it != result.end(); ++it) {
      auto pair = *it;
      EdgeValue sortedWeights = pair.second;
      std::sort(sortedWeights.begin(), sortedWeights.end());
      output.push_back({pair.first, sortedWeights});
    }

    std::sort(output.begin(), output.end(), [](const auto& a, const auto& b) {
      return a.first < b.first;
    });

    return output;
  }
}

