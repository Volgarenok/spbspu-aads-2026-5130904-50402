#include "graph.hpp"

#include <utility>
#include "table_utils.hpp"

bool samarin::Graph::hasVertex(const std::string& name) const
{
  return vertices_.has(name);
}

void samarin::Graph::addVertex(const std::string& name)
{
  insertOrGrow(vertices_, name, true);
}

void samarin::Graph::bind(const std::string& from, const std::string& to, Weight weight)
{
  addVertex(from);
  addVertex(to);
  const EdgeKey key(from, to);
  subscriptOrGrow(edges_, key).push_front(weight);
}

bool samarin::Graph::cut(const std::string& from, const std::string& to, Weight weight)
{
  const EdgeKey key(from, to);
  if (!edges_.has(key)) {
    return false;
  }
  WeightList& weights = edges_.at(key);
  WeightList kept;
  LIter< Weight > tail = kept.before_begin();
  bool removed = false;
  for (LIter< Weight > cur = weights.begin(); cur != weights.end(); ++cur) {
    if (!removed && *cur == weight) {
      removed = true;
      continue;
    }
    tail = kept.insert_after(tail, *cur);
  }
  if (!removed) {
    return false;
  }
  if (kept.empty()) {
    edges_.drop(key);
  } else {
    weights = std::move(kept);
  }
  return true;
}

void samarin::Graph::absorb(const Graph& other)
{
  for (auto it = other.vertices_.cbegin(); it != other.vertices_.cend(); ++it) {
    addVertex(it->first);
  }
  copyEdgesWithin(other);
}

void samarin::Graph::copyEdgesWithin(const Graph& source)
{
  for (auto it = source.edges_.cbegin(); it != source.edges_.cend(); ++it) {
    const std::string& from = it->first.first;
    const std::string& to = it->first.second;
    if (hasVertex(from) && hasVertex(to)) {
      for (auto w = it->second.cbegin(); w != it->second.cend(); ++w) {
        bind(from, to, *w);
      }
    }
  }
}

const samarin::Graph::VertexTable& samarin::Graph::vertices() const
{
  return vertices_;
}

const samarin::Graph::EdgeTable& samarin::Graph::edges() const
{
  return edges_;
}
