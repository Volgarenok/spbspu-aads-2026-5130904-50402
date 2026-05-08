#ifndef MATVEEV_GRAPH_COLLECTION_HPP
#define MATVEEV_GRAPH_COLLECTION_HPP

#include "graph.hpp"
#include "hashFunctions.hpp"
#include "hash-table.hpp"

#include <stdexcept>
#include <string>

namespace matveev
{
class GraphCollection
{
public:
  GraphCollection();

  bool hasGraph(const std::string& name) const;
  void addGraph(const std::string& name, const Graph& graph);
  Graph& at(const std::string& name);
  const Graph& at(const std::string& name) const;

private:
  HashTable< std::string, Graph, StringHash, StringEqual > graphs_;
};

inline GraphCollection::GraphCollection():
  graphs_(101, 4)
{}

inline bool GraphCollection::hasGraph(const std::string& name) const
{
  return graphs_.has(name);
}

inline void GraphCollection::addGraph(const std::string& name, const Graph& graph)
{
  if (graphs_.has(name))
  {
    throw std::logic_error("graph already exists");
  }

  graphs_.add(name, graph);
}

inline Graph& GraphCollection::at(const std::string& name)
{
  return graphs_.at(name);
}

inline const Graph& GraphCollection::at(const std::string& name) const
{
  return graphs_.at(name);
}
}

#endif
