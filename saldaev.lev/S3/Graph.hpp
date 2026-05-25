#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <string>
#include <boost/hash2/xxhash.hpp>
#include "../common/vector.hpp"
#include "HashTable.hpp"

namespace saldaev
{
  using StringPair = std::pair< std::string, std::string >;

  struct Graph
  {
  private:
    struct Hasher
    {
      size_t operator()(const StringPair &p) const;
    };

    struct KeyEqual
    {
      bool operator()(const StringPair &st, const StringPair &nd) const;
    };

    HashTable< StringPair, Vector< size_t >, Hasher, KeyEqual > edges;
    Vector< std::string > vertexes;

  public:
    using EdgeTable = HashTable< StringPair, Vector< size_t >, Hasher, KeyEqual >;

    Graph();

    Vector< std::string > vertices() const;
    bool hasVertex(const std::string &name) const;
    void addVertex(const std::string &name);

    bool hasEdge(const std::string &from, const std::string &to) const;
    void addEdge(const std::string &from, const std::string &to, size_t weight);
    void removeEdge(const std::string &from, const std::string &to, size_t weight);

    const Vector< std::pair< std::string, Vector< size_t > > > outgoingEdges(const std::string &name) const;
    const Vector< std::pair< std::string, Vector< size_t > > > incomingEdges(const std::string &name) const;
    const EdgeTable &getEdges() const;

    void clearEdges();
    void clear();
  };
}

namespace
{
  template< class T >
  saldaev::Vector< T > sort(const saldaev::Vector< T > &orig)
  {
    saldaev::Vector< T > v(orig);
    if (v.getSize() < 2)
      return v;

    bool swapped = true;
    while (swapped) {
      swapped = false;
      auto curr = v.begin();
      auto next = curr;
      ++next;
      while (next != v.end()) {
        if (*next < *curr) {
          std::swap(*curr, *next);
          swapped = true;
        }
        ++curr;
        ++next;
      }
    }
    return v;
  }
}

saldaev::Graph::Graph():
  edges(10, Hasher(), KeyEqual()),
  vertexes(0)
{}

saldaev::Vector< std::string > saldaev::Graph::vertices() const
{
  return vertexes;
}

bool saldaev::Graph::hasVertex(const std::string &name) const
{
  for (size_t i = 0; i < vertexes.getSize(); ++i) {
    if (vertexes[i] == name) {
      return true;
    }
  }
  return false;
}

void saldaev::Graph::addVertex(const std::string &name)
{
  for (size_t i = 0; i < vertexes.getSize(); ++i) {
    if (vertexes[i] == name) {
      throw std::logic_error("ununique name");
    }
  }
  vertexes.pushBack(name);
}

bool saldaev::Graph::hasEdge(const std::string &from, const std::string &to) const
{
  return edges.has({from, to});
}

void saldaev::Graph::addEdge(const std::string &from, const std::string &to, size_t weight)
{
  if (!hasEdge(from, to)) {
    edges.add({from, to}, Vector< size_t >(1));
  }
  edges.at({from, to}).pushBack(weight);
}

void saldaev::Graph::removeEdge(const std::string &from, const std::string &to, size_t weight)
{
  Vector< size_t > &v = edges.at({from, to});
  for (size_t i = 0; i < v.getSize(); ++i) {
    if (v[i] == weight) {
      v.erase(i);
      if (v.isEmpty()) {
        edges.remove({from, to});
      }
      return;
    }
  }
  throw std::logic_error("no such edge");
}

const saldaev::Vector< std::pair< std::string, saldaev::Vector< size_t > > >
saldaev::Graph::outgoingEdges(const std::string &name) const
{
  Vector< std::pair< std::string, Vector< size_t > > > ret(0);

  Vector< std::string > sVertexes = sort(vertexes);
  for (size_t i = 0; i < sVertexes.getSize(); ++i) {
    if (hasEdge(name, sVertexes[i])) {
      Vector< size_t > sortedWeights = sort(edges.get({name, sVertexes[i]}));
      ret.pushBack({sVertexes[i], sortedWeights});
    }
  }
  return ret;
}

const saldaev::Vector< std::pair< std::string, saldaev::Vector< size_t > > >
saldaev::Graph::incomingEdges(const std::string &name) const
{
  Vector< std::pair< std::string, Vector< size_t > > > ret(0);

  Vector< std::string > sVertexes = sort(vertexes);
  for (size_t i = 0; i < sVertexes.getSize(); ++i) {
    if (hasEdge(sVertexes[i], name)) {
      Vector< size_t > sortedWeights = sort(edges.get({sVertexes[i], name}));
      ret.pushBack({sVertexes[i], sortedWeights});
    }
  }
  return ret;
}

const saldaev::Graph::EdgeTable &saldaev::Graph::getEdges() const
{
  return edges;
}

void saldaev::Graph::clearEdges()
{
  edges.clear();
}

void saldaev::Graph::clear()
{
  edges.clear();
  vertexes.erase(0, vertexes.getSize());
}

size_t saldaev::Graph::Hasher::operator()(const StringPair &p) const
{
  boost::hash2::xxhash_64 hash;
  hash.update(p.first.data(), p.first.size());
  hash.update(p.second.data(), p.second.size());
  return static_cast< size_t >(hash.result());
}

bool saldaev::Graph::KeyEqual::operator()(const StringPair &st, const StringPair &nd) const
{
  return (st.first == nd.first) && (st.second == nd.second);
}

#endif
