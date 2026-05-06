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
#include <stdexcept>

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

  void swap(Graph& other) noexcept;

  bool hasVertex(const std::string& vertex) const;
  void addVertex(const std::string& vertex);
  bool hasEdge(const std::string& from, const std::string& to) const;
  void bind(const std::string& from, const std::string& to, unsigned long long weight);
  void cut(const std::string& from, const std::string& to, unsigned long long weight);
  const List< unsigned long long >& getWeights(const std::string& from, const std::string& to) const;
  const HashTable< std::string, bool, StringHash, StringEqual >& vertexes() const noexcept;
  const HashTable< EdgeKey, List< unsigned long long >, EdgeHash, EdgeEqual >& edges() const noexcept;

private:
  void bindUnsafe(const std::string& from, const std::string& to, unsigned long long weight);
  void cutUnsafe(const std::string& from, const std::string& to, unsigned long long weight);

  HashTable< std::string, bool, StringHash, StringEqual > vertexes_;
  HashTable< EdgeKey, List< unsigned long long >, EdgeHash, EdgeEqual > edges_;
};

inline Graph::Graph():
  vertexes_(101, 4),
  edges_(101, 4)
{}

inline void Graph::swap(Graph& other) noexcept
{
  vertexes_.swap(other.vertexes_);
  edges_.swap(other.edges_);
}
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
  Graph tmp(*this);
  tmp.bindUnsafe(from, to, weight);
  swap(tmp);
}

inline void Graph::bindUnsafe(const std::string& from, const std::string& to, unsigned long long weight)
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

inline void Graph::cut(const std::string& from, const std::string& to, unsigned long long weight)
{
  Graph tmp(*this);
  tmp.cutUnsafe(from, to, weight);
  swap(tmp);
}
inline void Graph::cutUnsafe(const std::string& from, const std::string& to, unsigned long long weight)
{
  if (!hasVertex(from) || !hasVertex(to))
  {
    throw std::logic_error("vertex not found");
  }

  EdgeKey key(from, to);

  if (!edges_.has(key))
  {
    throw std::logic_error("edge not found");
  }

  List< unsigned long long >& weights = edges_.at(key);
  LIter< unsigned long long > prev = weights.beforeBegin();
  LIter< unsigned long long > it = weights.begin();

  while (it != weights.end())
  {
    if (*it == weight)
    {
      weights.eraseAfter(prev);

      if (weights.begin() == weights.end())
      {
        edges_.drop(key);
      }

      return;
    }

    ++prev;
    ++it;
  }

  throw std::logic_error("weight not found");
}

inline const List< unsigned long long >& Graph::getWeights(const std::string& from, const std::string& to) const
{
  return edges_.at(EdgeKey(from, to));
}

inline const HashTable< std::string, bool, StringHash, StringEqual >& Graph::vertexes() const noexcept
{
  return vertexes_;
}

inline const HashTable< EdgeKey, List< unsigned long long >, EdgeHash, EdgeEqual >& Graph::edges() const noexcept
{
  return edges_;
}

}

#endif
