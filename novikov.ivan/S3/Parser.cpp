#include "Parser.hpp"
#include <stdexcept>

std::pair< novikov::graphTable, novikov::List< std::string > > novikov::parse(std::istream& in)
{
  novikov::graphTable gt;
  novikov::List< std::string > names;
  std::string name;
  while (in >> name)
  {
    sortedInsert(names, name);
    size_t count;
    in >> count;
    novikov::Graph graph;
    for (size_t i = 0; i < count; ++i)
    {
      std::string v1, v2;
      size_t weight;
      in >> v1 >> v2 >> weight;
      try
      {
        sortedInsert(graph.connections[{v1, v2}], weight);
      }
      catch (const std::runtime_error&)
      {
        graph.connections.rehash(graph.connections.size() * 2);
        sortedInsert(graph.connections[{v1, v2}], weight);
      }
      if (!has(graph.pairs, {v1, v2}))
      {
        sortedInsert(graph.pairs, {v1, v2});
      }
      if (!has(graph.vertexes, v1))
      {
        sortedInsert(graph.vertexes, v1);
      }
      if (!has(graph.vertexes, v2))
      {
        sortedInsert(graph.vertexes, v2);
      }
    }
    try
    {
      gt.insert(name, graph);
    }
    catch (const std::runtime_error&)
    {
      gt.rehash(gt.size() * 2);
      gt.insert(name, graph);
    }
  }
  return {gt, names};
}

template< class T >
bool novikov::has(const List< T >& list, const T& elem)
{
  for (auto it = list.begin(); it != list.end(); ++it)
  {
    if ((*it) == elem)
    {
      return true;
    }
  }
  return false;
}

template< class T >
void novikov::sortedInsert(List< T >& list, const T& value)
{
  bool isInserted = false;
  for (auto it = list.begin(); it != list.end(); ++it)
  {
    if (*it > value)
    {
      list.insert(it, value);
      isInserted = true;
      break;
    }
  }
  if (!isInserted)
  {
    list.pushBack(value);
  }
}
