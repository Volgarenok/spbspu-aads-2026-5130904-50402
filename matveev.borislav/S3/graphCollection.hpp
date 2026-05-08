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

  void swap(GraphCollection& other) noexcept;
  bool hasGraph(const std::string& name) const;
  void addGraph(const std::string& name, const Graph& graph);
  void createGraph(const std::string& name, const List< std::string >& vertexes);
  void mergeGraphs(const std::string& name, const std::string& lhs, const std::string& rhs);
  void extractGraph(const std::string& name, const std::string& old_name, const List< std::string >& vertexes);
  Graph& at(const std::string& name);
  const Graph& at(const std::string& name) const;

private:
  HashTable< std::string, Graph, StringHash, StringEqual > graphs_;
};

inline GraphCollection::GraphCollection():
  graphs_(101, 4)
{}

inline void GraphCollection::swap(GraphCollection& other) noexcept
{
  graphs_.swap(other.graphs_);
}

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

inline void GraphCollection::createGraph(const std::string& name, const List< std::string >& vertexes)
{
  if (graphs_.has(name))
  {
    throw std::logic_error("graph already exists");
  }

  Graph graph;

  for (LCIter< std::string > it = vertexes.begin(); it != vertexes.end(); ++it)
  {
    graph.addVertex(*it);
  }

  GraphCollection tmp(*this);
  tmp.graphs_.add(name, graph);
  swap(tmp);
}

inline void GraphCollection::mergeGraphs(const std::string& name, const std::string& lhs, const std::string& rhs)
{
  if (graphs_.has(name))
  {
    throw std::logic_error("graph already exists");
  }

  if (!graphs_.has(lhs) || !graphs_.has(rhs))
  {
    throw std::logic_error("graph not found");
  }

  Graph result = graphs_.at(lhs);
  result.append(graphs_.at(rhs));

  GraphCollection tmp(*this);
  tmp.graphs_.add(name, result);
  swap(tmp);
}

inline void GraphCollection::extractGraph(
  const std::string& name,
  const std::string& old_name,
  const List< std::string >& vertexes
)
{
  if (graphs_.has(name))
  {
    throw std::logic_error("graph already exists");
  }

  if (!graphs_.has(old_name))
  {
    throw std::logic_error("graph not found");
  }

  Graph result = graphs_.at(old_name).extract(vertexes);

  GraphCollection tmp(*this);
  tmp.graphs_.add(name, result);
  swap(tmp);
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
