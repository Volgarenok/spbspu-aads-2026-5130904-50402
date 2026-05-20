#ifndef MATVEEV_GRAPH_ALGORITHMS_HPP
#define MATVEEV_GRAPH_ALGORITHMS_HPP

#include "graphCollection.hpp"

#include "../common/list.hpp"

#include <string>
#include <stdexcept>
#include <ostream>

namespace matveev
{
inline void insertSorted(List< std::string >& list, const std::string& value)
{
  LIter< std::string > prev = list.beforeBegin();
  LIter< std::string > it = list.begin();

  while (it != list.end() && *it < value)
  {
    ++prev;
    ++it;
  }

  list.insertAfter(prev, value);
}

struct EdgeOutput
{
  EdgeOutput();
  explicit EdgeOutput(const std::string& vertex_value);

  std::string vertex;
  List< unsigned long long > weights;
};

inline EdgeOutput::EdgeOutput():
  vertex(),
  weights()
{}

inline EdgeOutput::EdgeOutput(const std::string& vertex_value):
  vertex(vertex_value),
  weights()
{}

inline void insertSorted(List< unsigned long long >& list, unsigned long long value)
{
  LIter< unsigned long long > prev = list.beforeBegin();
  LIter< unsigned long long > it = list.begin();

  while (it != list.end() && *it < value)
  {
    ++prev;
    ++it;
  }

  list.insertAfter(prev, value);
}

inline void addEdgeOutput(List< EdgeOutput >& list, const std::string& vertex, unsigned long long weight)
{
  LIter< EdgeOutput > prev = list.beforeBegin();
  LIter< EdgeOutput > it = list.begin();

  while (it != list.end() && it->vertex < vertex)
  {
    ++prev;
    ++it;
  }

  if (it != list.end() && it->vertex == vertex)
  {
    insertSorted(it->weights, weight);
    return;
  }

  EdgeOutput output(vertex);
  insertSorted(output.weights, weight);
  list.insertAfter(prev, output);
}

inline List< EdgeOutput > collectOutboundEdges(const Graph& graph, const std::string& vertex)
{
  if (!graph.hasVertex(vertex))
  {
    throw std::logic_error("vertex not found");
  }

  List< EdgeOutput > result;

  for (auto edge_it = graph.edges().cbegin(); edge_it != graph.edges().cend(); ++edge_it)
  {
    const EdgeKey& key = edge_it->key;

    if (key.from == vertex)
    {
      const List< unsigned long long >& weights = edge_it->value;

      for (LCIter< unsigned long long > weight_it = weights.begin(); weight_it != weights.end(); ++weight_it)
      {
        addEdgeOutput(result, key.to, *weight_it);
      }
    }
  }

  return result;
}

inline List< EdgeOutput > collectInboundEdges(const Graph& graph, const std::string& vertex)
{
  if (!graph.hasVertex(vertex))
  {
    throw std::logic_error("vertex not found");
  }

  List< EdgeOutput > result;

  for (auto edge_it = graph.edges().cbegin(); edge_it != graph.edges().cend(); ++edge_it)
  {
    const EdgeKey& key = edge_it->key;

    if (key.to == vertex)
    {
      const List< unsigned long long >& weights = edge_it->value;

      for (LCIter< unsigned long long > weight_it = weights.begin(); weight_it != weights.end(); ++weight_it)
      {
        addEdgeOutput(result, key.from, *weight_it);
      }
    }
  }

  return result;
}

inline List< std::string > collectGraphNames(const GraphCollection& graphs)
{
  List< std::string > result;

  for (auto it = graphs.graphs().cbegin(); it != graphs.graphs().cend(); ++it)
  {
    insertSorted(result, it->key);
  }

  return result;
}

inline List< std::string > collectVertexNames(const Graph& graph)
{
  List< std::string > result;

  for (auto it = graph.vertexes().cbegin(); it != graph.vertexes().cend(); ++it)
  {
    insertSorted(result, it->key);
  }

  return result;
}

inline void printStringList(std::ostream& out, const List< std::string >& list)
{
  for (LCIter< std::string > it = list.begin(); it != list.end(); ++it)
  {
    out << *it << '\n';
  }

  if (list.begin() == list.end())
  {
    out << '\n';
  }
}

inline void printEdgeRows(std::ostream& out, const List< EdgeOutput >& rows)
{
  for (LCIter< EdgeOutput > it = rows.begin(); it != rows.end(); ++it)
  {
    out << it->vertex;

    for (LCIter< unsigned long long > weight = it->weights.begin(); weight != it->weights.end(); ++weight)
    {
      out << ' ' << *weight;
    }

    out << '\n';
  }

  if (rows.begin() == rows.end())
  {
    out << '\n';
  }
}
}

#endif
