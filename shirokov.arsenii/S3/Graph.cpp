#include "Graph.hpp"
#include <bilist.hpp>
#include <stdexcept>
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
  size_t weight;
  in >> v1 >> v2 >> weight;
  try
  {
    sortSaveInsertion(cpy.connections[{v1, v2}], weight);
  }
  catch (const std::runtime_error&)
  {
    cpy.connections.rehash(cpy.connections.size() * 2);
    sortSaveInsertion(cpy.connections[{v1, v2}], weight);
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

void shirokov::cut(std::ostream& out, std::istream& in, graphTable& gt, BiList< std::string >&)
{
  std::string graphName;
  in >> graphName;
  Graph cpy = gt.at(graphName);
  std::string v1, v2;
  size_t weight;
  in >> v1 >> v2 >> weight;
}
