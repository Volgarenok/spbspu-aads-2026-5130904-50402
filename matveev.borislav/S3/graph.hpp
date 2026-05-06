#ifndef MATVEEV_GRAPH_HPP
#define MATVEEV_GRAPH_HPP

#include "hash-table.hpp"
#include "hashFunctions.hpp"
#include "../common/list.hpp"

#include <boost/hash2/flavor.hpp>
#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/xxhash.hpp>

#include <cstddef>
#include <string>

namespace matveev
{
struct EdgeKey
{
  EdgeKey();
  EdgeKey(const std::string& from_value, const std::string& to_value);

  std::string from;
  std::string to;
};

inline EdgeKey::EdgeKey():
  from(),
  to()
{}

inline EdgeKey::EdgeKey(const std::string& from_value, const std::string& to_value):
  from(from_value),
  to(to_value)
{}

template< class Provider, class Hash, class Flavor >
void tag_invoke(
  boost::hash2::hash_append_tag const&,
  const Provider&,
  Hash& hash,
  const Flavor& flavor,
  const EdgeKey* key
)
{
  boost::hash2::hash_append(hash, flavor, key->from.size());
  boost::hash2::hash_append(hash, flavor, key->from);
  boost::hash2::hash_append(hash, flavor, key->to.size());
  boost::hash2::hash_append(hash, flavor, key->to);
}

struct EdgeHash
{
  size_t operator()(const EdgeKey& key) const
  {
    boost::hash2::xxhash_64 hash;
    boost::hash2::hash_append(hash, boost::hash2::default_flavor(), key);

    return static_cast< size_t >(hash.result());
  }
};

struct EdgeEqual
{
  bool operator()(const EdgeKey& lhs, const EdgeKey& rhs) const
  {
    return lhs.from == rhs.from && lhs.to == rhs.to;
  }
};

class Graph
{
public:
  Graph();

  bool hasVertex(const std::string& vertex) const;
  void addVertex(const std::string& vertex);
  bool hasEdge(const std::string& from, const std::string& to) const;
  void bind(const std::string& from, const std::string& to, unsigned long long weight);
  const List< unsigned long long >& getWeights(const std::string& from, const std::string& to) const;

private:
  HashTable< std::string, bool, StringHash, StringEqual > vertexes_;
  HashTable< EdgeKey, List< unsigned long long >, EdgeHash, EdgeEqual > edges_;
};

inline Graph::Graph():
  vertexes_(101, 4),
  edges_(101, 4)
{}

inline bool Graph::hasVertex(const std::string& vertex) const
{
  return vertexes_.has(vertex);
}

inline void Graph::addVertex(const std::string& vertex)
{
  if (!vertexes_.has(vertex))
  {
    vertexes_.add(vertex, true);
  }
}

inline bool Graph::hasEdge(const std::string& from, const std::string& to) const
{
  return edges_.has(EdgeKey(from, to));
}

inline void Graph::bind(const std::string& from, const std::string& to, unsigned long long weight)
{
  addVertex(from);
  addVertex(to);

  EdgeKey key(from, to);

  if (edges_.has(key))
  {
    List< unsigned long long >& weights = edges_.at(key);
    weights.insertAfter(weights.beforeBegin(), weight);
    return;
  }

  List< unsigned long long > weights;
  weights.insertAfter(weights.beforeBegin(), weight);
  edges_.add(key, weights);
}

inline const List< unsigned long long >& Graph::getWeights(const std::string& from, const std::string& to) const
{
  return edges_.at(EdgeKey(from, to));
}

}

#endif
