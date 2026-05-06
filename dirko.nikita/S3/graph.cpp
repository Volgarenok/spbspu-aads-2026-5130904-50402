#include "graph.hpp"
#include <utility>

dirko::Graph::Graph() noexcept:
  vertices(),
  conections(20)
{}

dirko::Graph::Graph(size_t edgeSlots):
  vertices(),
  conections(edgeSlots)
{}

void dirko::Graph::addVertex(const std::string &vl)
{
  for (const std::string &v : vertices) {
    if (v == vl) {
      return;
    }
  }
  vertices.push_back(vl);
}

void dirko::Graph::removeVertex(const std::string &vl)
{
  bool found = false;
  for (LIter< std::string > it = vertices.begin(); it != vertices.end(); ++it) {
    if (*it == vl) {
      vertices.erase(it);
      found = true;
      break;
    }
  }
  if (!found) {
    throw std::out_of_range("Vertex not found");
  }
  Vector< std::pair< std::string, std::string > > to_remove;
  for (const std::pair< std::pair< std::string, std::string >, List< size_t > > &v : conections) {
    if ((v.first.first == vl) || (v.first.second == vl)) {
      to_remove.pushBack(v.first);
    }
  }
  for (size_t i = 0; i < to_remove.getSize(); ++i) {
    conections.drop(to_remove[i]);
  }
}

void dirko::Graph::addConection(const std::string &from, const std::string &to, size_t weight)
{
  addVertex(from);
  addVertex(to);
  std::pair< std::string, std::string > key(from, to);
  if (conections.has(key)) {
    conections.get(key).push_back(weight);
  } else {
    List< size_t > weights;
    weights.push_back(weight);
    conections.add(key, weights);
  }
}

void dirko::Graph::removeConection(const std::string &from, const std::string &to, size_t weight)
{
  std::pair< std::string, std::string > key(from, to);
  if (!conections.has(key)) {
    throw std::out_of_range("Edge not found");
  }
  List< size_t > &weights = conections.get(key);
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
  if (weights.size() == 0) {
    conections.drop(key);
  }
}
