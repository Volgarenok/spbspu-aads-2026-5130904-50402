#include "Graph.hpp"
#include <bilist.hpp>
#include <stdexcept>
#include <string>
#include <utility>
#include "functions.hpp"

void shirokov::graphs(std::ostream& out, std::istream&, graphTable&, BiList< std::string >& names)
{
  for (auto it = names.begin(); it != names.end(); ++it)
  {
    out << *it << '\n';
  }
  if (names.empty())
  {
    out << '\n';
  }
}

void shirokov::vertexes(std::ostream& out, std::istream& in, graphTable& gt, BiList< std::string >&)
{
  std::string graphName;
  in >> graphName;
  if (!gt.contains(graphName))
  {
    throw std::logic_error("Undefined graph name");
  }
  for (auto it = gt.at(graphName).vertexes.begin(); it != gt.at(graphName).vertexes.end(); ++it)
  {
    out << *it << '\n';
  }
  if (gt.at(graphName).vertexes.empty())
  {
    out << '\n';
  }
}

void shirokov::outbound(std::ostream& out, std::istream& in, graphTable& gt, BiList< std::string >&)
{
  std::string graphName, vertexName;
  in >> graphName >> vertexName;
  const Graph& graph = gt.at(graphName);
  if (!has(graph.vertexes, vertexName))
  {
    throw std::runtime_error("Undefined vertex");
  }
  for (auto it = graph.pairs.begin(); it != graph.pairs.end(); ++it)
  {
    if ((*it).first == vertexName)
    {
      out << (*it).second;
      for (auto weightIt = graph.connections.at(*it).begin(); weightIt != graph.connections.at(*it).end(); ++weightIt)
      {
        out << ' ' << *weightIt;
      }
      out << '\n';
    }
  }
}

void shirokov::inbound(std::ostream& out, std::istream& in, graphTable& gt, BiList< std::string >&)
{
  std::string graphName, vertexName;
  in >> graphName >> vertexName;
  const Graph& graph = gt.at(graphName);
  if (!has(graph.vertexes, vertexName))
  {
    throw std::runtime_error("Undefined vertex");
  }
  for (auto it = graph.pairs.begin(); it != graph.pairs.end(); ++it)
  {
    if ((*it).second == vertexName)
    {
      out << (*it).first;
      for (auto weightIt = graph.connections.at(*it).begin(); weightIt != graph.connections.at(*it).end(); ++weightIt)
      {
        out << ' ' << *weightIt;
      }
      out << '\n';
    }
  }
}

void shirokov::bind(std::ostream&, std::istream& in, graphTable& gt, BiList< std::string >&)
{
  std::string graphName;
  in >> graphName;
  Graph cpy = gt.at(graphName);
  std::string v1, v2;
  size_t w;
  in >> v1 >> v2 >> w;
  try
  {
    sortSaveInsertion(cpy.connections[{v1, v2}], w);
  }
  catch (const std::runtime_error&)
  {
    cpy.connections.rehash(cpy.connections.size() * 2);
    sortSaveInsertion(cpy.connections[{v1, v2}], w);
  }
  if (!has(cpy.pairs, {v1, v2}))
  {
    sortSaveInsertion(cpy.pairs, {v1, v2});
  }
  if (!has(cpy.vertexes, v1))
  {
    sortSaveInsertion(cpy.vertexes, v1);
  }
  if (!has(cpy.vertexes, v2))
  {
    sortSaveInsertion(cpy.vertexes, v2);
  }
  std::swap(cpy, gt.at(graphName));
}

void shirokov::cut(std::ostream&, std::istream& in, graphTable& gt, BiList< std::string >&)
{
  std::string graphName;
  in >> graphName;
  Graph cpy = gt.at(graphName);
  std::string v1, v2;
  size_t w;
  in >> v1 >> v2 >> w;
  BiList< size_t >& weights = cpy.connections.at({v1, v2});
  bool isErased = false;
  for (auto it = weights.begin(); it != weights.end(); ++it)
  {
    if (*it == w)
    {
      weights.erase(it);
      isErased = true;
      break;
    }
  }
  if (!isErased)
  {
    throw std::runtime_error("Undefined weight");
  }
  if (weights.empty())
  {
    cpy.connections.erase({v1, v2});
    for (auto it = cpy.pairs.begin(); it != cpy.pairs.end(); ++it)
    {
      if (*it == std::pair< std::string, std::string >(v1, v2))
      {
        cpy.pairs.erase(it);
        break;
      }
    }
  }
  bool isContainsV1 = false, isContainsV2 = false;
  for (auto it = cpy.pairs.begin(); it != cpy.pairs.end(); ++it)
  {
    if ((*it).first == v1 || (*it).second == v1)
    {
      isContainsV1 = true;
    }
    if ((*it).first == v2 || (*it).second == v2)
    {
      isContainsV2 = true;
    }
    if (isContainsV1 && isContainsV2)
    {
      break;
    }
  }
  if (!isContainsV1)
  {
    for (auto it = cpy.vertexes.begin(); it != cpy.vertexes.end(); ++it)
    {
      if (*it == v1)
      {
        cpy.vertexes.erase(it);
        break;
      }
    }
  }
  if (!isContainsV2)
  {
    for (auto it = cpy.vertexes.begin(); it != cpy.vertexes.end(); ++it)
    {
      if (*it == v2)
      {
        cpy.vertexes.erase(it);
        break;
      }
    }
  }

  std::swap(cpy, gt.at(graphName));
}
