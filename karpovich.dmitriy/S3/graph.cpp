#include "graph.hpp"

karpovich::Graph::Graph() noexcept:
  vertices(),
  edges(16)
{}

karpovich::Graph::Graph(size_t edgeSlots):
  vertices(),
  edges(edgeSlots)
{}

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
    vertices.pushBack(v);
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
  Vector< std::pair< std::string, std::string > > to_remove;
  for (HashIter< std::pair< std::string, std::string >, List< size_t >, PairHasher< std::string >,
                 std::equal_to< std::pair< std::string, std::string > > >
           it = edges.begin();
       it != edges.end(); ++it) {
    if (((*it).first.first == v) || ((*it).first.second == v)) {
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
  std::pair< std::string, std::string > key(from, to);
  if (edges.has(key)) {
    edges.get(key).pushBack(weight);
  } else {
    List< size_t > weights;
    weights.pushBack(weight);
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
  LIter< size_t > it = weights.begin();
  while (it != weights.end()) {
    if (*it == weight) {
      it = weights.erase(it);
      found = true;
      break;
    } else {
      ++it;
    }
  }
  if (!found) {
    throw std::out_of_range("Weight not found");
  }
  if (weights.empty()) {
    edges.drop(key);
  }
}
