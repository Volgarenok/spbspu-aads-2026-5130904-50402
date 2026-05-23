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
  bool isOutput = false;
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
      isOutput = true;
    }
  }
  if (!isOutput)
  {
    out << '\n';
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
  bool isOutput = false;
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
      isOutput = true;
    }
  }
  if (!isOutput)
  {
    out << '\n';
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

  std::swap(cpy, gt.at(graphName));
}

void shirokov::create(std::ostream&, std::istream& in, graphTable& gt, BiList< std::string >& names)
{
  std::string graphName;
  in >> graphName;
  if (gt.contains(graphName))
  {
    throw std::runtime_error("This graph already exists");
  }
  Graph graph;
  size_t count;
  in >> count;
  for (size_t i = 0; i < count; ++i)
  {
    std::string vertex;
    in >> vertex;
    if (!has(graph.vertexes, vertex))
    {
      sortSaveInsertion(graph.vertexes, vertex);
    }
  }
  try
  {
    gt.insert(graphName, graph);
  }
  catch (const std::runtime_error&)
  {
    gt.rehash(gt.size() * 2);
    gt.insert(graphName, graph);
  }
  sortSaveInsertion(names, graphName);
}

void shirokov::merge(std::ostream&, std::istream& in, graphTable& gt, BiList< std::string >& names)
{
  std::string graphName, grName1, grName2;
  in >> graphName >> grName1 >> grName2;
  if (gt.contains(graphName) || !gt.contains(grName1) || !gt.contains(grName2))
  {
    throw std::runtime_error("Invalid graph name(s)");
  }

  Graph graph, gr1 = gt.at(grName1), gr2 = gt.at(grName2);
  graph.vertexes = gr1.vertexes;
  for (auto it = gr2.vertexes.begin(); it != gr2.vertexes.end(); ++it)
  {
    if (!has(graph.vertexes, *it))
    {
      sortSaveInsertion(graph.vertexes, *it);
    }
  }

  graph.pairs = gr1.pairs;
  for (auto it = gr2.pairs.begin(); it != gr2.pairs.end(); ++it)
  {
    if (!has(graph.pairs, *it))
    {
      sortSaveInsertion(graph.pairs, *it);
    }
  }

  for (auto it = gr1.connections.begin(); it != gr1.connections.end(); ++it)
  {
    BiList< size_t >* list = nullptr;
    try
    {
      list = &graph.connections[(*it).first];
    }
    catch (const std::runtime_error&)
    {
      graph.connections.rehash(graph.connections.size() * 2);
      list = &graph.connections[(*it).first];
    }
    for (auto wIt = (*it).second.begin(); wIt != (*it).second.end(); ++wIt)
    {
      sortSaveInsertion(*list, *wIt);
    }
  }

  for (auto it = gr2.connections.begin(); it != gr2.connections.end(); ++it)
  {
    BiList< size_t >* list = nullptr;
    try
    {
      list = &graph.connections[(*it).first];
    }
    catch (const std::runtime_error&)
    {
      graph.connections.rehash(graph.connections.size() * 2);
      list = &graph.connections[(*it).first];
    }
    for (auto wIt = (*it).second.begin(); wIt != (*it).second.end(); ++wIt)
    {
      sortSaveInsertion(*list, *wIt);
    }
  }

  try
  {
    gt.insert(graphName, graph);
  }
  catch (const std::runtime_error&)
  {
    gt.rehash(gt.size() * 2);
    gt.insert(graphName, graph);
  }
  sortSaveInsertion(names, graphName);
}

void shirokov::extract(std::ostream&, std::istream& in, graphTable& gt, BiList< std::string >& names)
{
  std::string newGraphName, oldGraphName;
  in >> newGraphName >> oldGraphName;
  if (gt.contains(newGraphName) || !gt.contains(oldGraphName))
  {
    throw std::runtime_error("Invalid graph name(s)");
  }
  Graph graph, oldGraph = gt.at(oldGraphName);
  size_t count;
  in >> count;
  for (size_t i = 0; i < count; ++i)
  {
    std::string vertex;
    in >> vertex;
    if (!has(oldGraph.vertexes, vertex))
    {
      throw std::runtime_error("Invalid vertex");
    }
    sortSaveInsertion(graph.vertexes, vertex);
  }

  for (auto it = oldGraph.pairs.begin(); it != oldGraph.pairs.end(); ++it)
  {
    if (has(graph.vertexes, (*it).first) && has(graph.vertexes, (*it).second))
    {
      sortSaveInsertion(graph.pairs, (*it));
    }
  }

  for (auto it = graph.pairs.begin(); it != graph.pairs.end(); ++it)
  {
    try
    {
      graph.connections[*it] = oldGraph.connections[*it];
    }
    catch (const std::runtime_error&)
    {
      graph.connections.rehash(graph.connections.size() * 2);
      graph.connections[*it] = oldGraph.connections[*it];
    }
  }

  gt.insert(newGraphName, graph);
  sortSaveInsertion(names, newGraphName);
}
