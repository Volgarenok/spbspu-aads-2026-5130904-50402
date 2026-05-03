#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <cstddef>
#include <stdexcept>
#include <string>
#include <utility>
#include "../common/Vector.hpp"
#include "../common/list.hpp"
#include "hashFunctions.hpp"
#include "hashIters.hpp"
#include "hashTable.hpp"

namespace karpovich
{
  struct Graph
  {
    using str_t = std::pair< std::string, std::string >;
    List< std::string > vertices;
    HashTable< str_t, List< size_t >, PairHasher< std::string >, std::equal_to< str_t > > edges;

    explicit Graph(size_t edgeSlots);

    void addVertex(const std::string &v);
    void removeVertex(const std::string &v);
    void addEdge(const std::string &from, const std::string &to, size_t weight);
    void removeEdge(const std::string &from, const std::string &to, size_t weight);
  };
}

void karpovich::Graph::addVertex(const std::string &v)
{
  bool exists = false;
  for (LIter< std::string > it = vertices.begin(); it != vertices.end(); ++it) {
    if (*it == v) {
      exists = true;
      break;
    }
  }
  if (!exists) {
    vertices.push_back(v);
  }
}

void karpovich::Graph::removeVertex(const std::string &v)
{
  bool found = false;
  for (LIter< std::string > it = vertices.begin(); it != vertices.end(); ++it) {
    if (*it == v) {
      vertices.erase(it);
      found = true;
      break;
    }
  }
  if (!found) {
    throw std::out_of_range("Vertex not found");
  }
  Vector< str_t > to_remove;
  for (HashIter< str_t, List< size_t >, PairHasher< std::string >, std::equal_to< str_t > > it = edges.begin();
       it != edges.end(); ++it) {
    if ((*it).first.first == v || (*it).first.second == v) {
      to_remove.pushBack((*it).first);
    }
  }
  for (size_t i = 0; i < to_remove.getSize(); ++i) {
    edges.drop(to_remove[i]);
  }
}

void karpovich::Graph::addEdge(const std::string &from, const std::string &to, size_t weight)
{
  addVertex(from);
  addVertex(to);
  str_t key(from, to);
  if (edges.has(key)) {
    edges.get(key).push_back(weight);
  } else {
    List< size_t > weights;
    weights.push_back(weight);
    edges.add(key, weights);
  }
}

void karpovich::Graph::removeEdge(const std::string &from, const std::string &to, size_t weight)
{
  std::pair< std::string, std::string > key(from, to);
  if (!edges.has(key)) {
    throw std::out_of_range("Edge not found");
  }
  List< size_t > &weights = edges.get(key);
  bool found = false;
  for (LIter< size_t > it = weights.begin(); it != weights.end(); ++it) {
    if (*it == weight) {
      weights.erase(it);
      found = true;
      break;
    }
  }
  if (!found) {
    throw std::out_of_range("Weight not found");
  }
  if (weights.empty()) {
    edges.drop(key);
  }
}

#endif
