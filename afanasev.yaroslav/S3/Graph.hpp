#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <set>
#include <string>
#include <stdexcept>
#include "HashTable/HashTable.hpp"
#include "HashTable/HashIter.hpp"
#include "HashTable/HashFunction.hpp"
#include "list.hpp"
#include "top-it-vector.hpp"

namespace afanasev
{
  class Graph
  {
    public:
    Graph();
    explicit Graph(size_t edgeSlots);

    void addVertex(const std::string & v);
    bool hasVertex(const std::string & v) const;
    const std::set< std::string > & getVertices() const;

    void addEdge(const std::string& from, const std::string & to, int weight);
    void removeEdge(const std::string& from, const std::string & to, int weight);

    Vector< std::pair< std::string, int > > getOutEdges(const std::string & vertex) const;
    Vector< std::pair< std::string, int > > getInEdges(const std::string & vertex) const;

    private:
    std::set< std::string > vertices_;
    HashTable< std::pair< std::string, std::string >,
    List< int >,
    PairHasher< std::string >,
    std::equal_to< std::pair< std::string, std::string > > > edges_;
  };
}

afanasev::Graph::Graph():
  vertices_(),
  edges_(32)
{}

afanasev::Graph::Graph(size_t edgeSlots):
  vertices_(),
  edges_(edgeSlots)
{}

void afanasev::Graph::addVertex(const std::string & v)
{
  vertices_.insert(v);
}

bool afanasev::Graph::hasVertex(const std::string & v) const
{
  return (vertices_.find(v) != vertices_.end());
}

const std::set< std::string > & afanasev::Graph::getVertices() const
{
  return vertices_;
}

void afanasev::Graph::addEdge(const std::string & from, const std::string & to, int weight)
{
  addVertex(from);
  addVertex(to);

  std::pair< std::string, std::string > key(from, to);
  if (edges_.has(key))
  {
    edges_.get(key).pushFront(weight);
  }
  else
  {
    List< int > weights;
    weights.pushFront(weight);
    edges_.add(key, weights);
  }
}

void afanasev::Graph::removeEdge(const std::string & from, const std::string & to, int weight)
{
  std::pair< std::string, std::string > key(from, to);
  if (!edges_.has(key))
  {
    throw std::out_of_range("Edge not found");
  }

  List< int > & weights = edges_.get(key);

  List< int > newWeights;
  LIter< int > it = weights.begin();
  bool removed = false;

  while (it != LIter< int >())
  {
    if (*it == weight)
    {
      removed = true;
    }
    else
    {
      newWeights.pushFront(*it);
    }
    ++it;
  }
  if (!removed)
  {
    throw std::out_of_range("Weight not found");
  }
  weights = newWeights;

  if (weights.begin() == LIter< int >())
  {
    edges_.drop(key);
  }
}

afanasev::Vector< std::pair< std::string, int > >
afanasev::Graph::getOutEdges(const std::string & vertex) const
{
  afanasev::Vector< std::pair< std::string, int > > result;

  for (HashConstIter< std::pair< std::string, std::string >, List< int >, PairHasher< std::string >,
    std::equal_to< std::pair< std::string, std::string > > > it = edges_.cbegin(); it != edges_.cend(); ++it)
  {
    const std::pair< std::string, std::string > & key = (*it).first;
    if (key.first == vertex)
    {
      const List< int > & weights = (*it).second;
      LCIter< int > wit = weights.begin();
      while (wit != LCIter< int >())
      {
        result.pushBack(std::make_pair(key.second, *wit));
        ++wit;
      }
    }
  }
  return result;
}

afanasev::Vector< std::pair< std::string, int > >
afanasev::Graph::getInEdges(const std::string& vertex) const
{
  afanasev::Vector< std::pair< std::string, int > > result;
  for (HashConstIter< std::pair< std::string, std::string >, List< int >, PairHasher< std::string >,
    std::equal_to< std::pair< std::string, std::string > > > it = edges_.cbegin(); it != edges_.cend(); ++it)
  {
    const std::pair< std::string, std::string > & key = (*it).first;
    if (key.second == vertex)
    {
      const List< int > & weights = (*it).second;
      LCIter< int > wit = weights.begin();
      while (wit != LCIter< int >())
      {
        result.pushBack(std::make_pair(key.first, *wit));
        ++wit;
      }
    }
  }
  return result;
}

#endif
