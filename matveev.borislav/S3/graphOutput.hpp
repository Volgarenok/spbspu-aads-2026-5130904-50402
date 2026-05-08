#ifndef MATVEEV_GRAPH_ALGORITHMS_HPP
#define MATVEEV_GRAPH_ALGORITHMS_HPP

#include "graphCollection.hpp"

#include "../common/list.hpp"

#include <string>

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
}

#endif
