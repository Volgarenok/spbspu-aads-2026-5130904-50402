#include "graph.hpp"

namespace muh = muhamadiarov;

muh::Graph::Graph():
  vertices_(),
  bonds_(5, 5)
{}

muh::Graph::Graph(size_t bucketCapacity, size_t bucketCounts):
  vertices_(),
  bonds_(bucketCapacity, bucketCounts)
{}

bool muh::Graph::findV(const std::string& vertex) const
{
  LCIter< std::string  > iter = vertices_.cbegin();
  for (size_t i = 0; i < vertices_.size(); ++i)
  {
    if (*iter == vertex)
    {
      return true;
    }
    ++iter;
  }
  return false;
}

bool muh::Graph::findC(const std::string& from, const std::string& to, size_t weight)
{
  key_t pair{from, to};
  if (!bonds_.has(pair))
  {
    return false;
  }
  List< size_t >& list = bonds_.get(pair);
  LCIter< size_t > iter = list.cbegin();
  for (size_t i = 0; i < list.size(); ++i)
  {
    if (*iter == weight)
    {
      return true;
    }
    ++iter;
  }
  return false;
}

void muh::Graph::addVertex(const std::string& vertex)
{
  if (findV(vertex))
  {
    return;
  }
  vertices_.pushBack(vertex);
}

void muh::Graph::removeVertex(const std::string& vertex)
{
  LIter< std::string > iter = vertices_.begin();
  bool found = false;
  for (size_t i = 0; i < vertices_.size(); ++i)
  {
    if (*iter == vertex)
    {
      iter = vertices_.erase(iter);
      found = true;
    }
    else
    {
      ++iter;
    }
  }
  if (!found)
  {
    throw std::out_of_range("Vertex not found");
  }
  List< key_t > toRemove;
  HTIter< key_t, List<size_t>, PairHash, std::equal_to<key_t> > it = bonds_.begin();
  while (it != bonds_.end())
  {
    const key_t& key = it->key_;
    if (key.first == vertex || key.second == vertex)
    {
      toRemove.pushBack(key);
    }
    ++it;
  }
  LIter< key_t > rmIt = toRemove.begin();
  for (size_t i = 0; i < toRemove.size(); ++i)
  {
    bonds_.drop(*rmIt);
    ++rmIt;
  }
}

void muh::Graph::addConnection(const std::string& from, const std::string& to, size_t weight)
{
  addVertex(from);
  addVertex(to);
  key_t pair{from, to};

  if (bonds_.has(pair))
  {
    List<size_t>& list = bonds_.get(pair);
    list.pushFront(weight);
  }
  else
  {
    List<size_t> newList;
    newList.pushFront(weight);
    bonds_.add(pair, newList);
  }
}

void muh::Graph::removeConnection(const std::string& from, const std::string& to, size_t weight)
{
  key_t pair{from, to};
  if (!bonds_.has(pair))
  {
    throw std::out_of_range("There is not this connection");
  }
  List< size_t >& list = bonds_.get(pair);
  LIter< size_t > iter = list.begin();
  bool found = false;
  for (size_t i = 0; i < list.size(); ++i)
  {
    if (*iter == weight)
    {
      iter = list.erase(iter);
      found = true;
      break;
    }
    else
    {
      ++iter;
    }
  }
  if (!found)
  {
    throw std::out_of_range("weight not found");
  }
  if (list.size() == 0)
  {
    List< size_t > removed = bonds_.drop(pair);
  }
}
