#include <stdexcept>
#include <string>
#include <utility>
#include <bilist.hpp>
#include "Graph.hpp"
#include "Parser.hpp"

void novikov::graphs(std::ostream& out, std::istream&, graphTable&, List< std::string >& names)
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

void novikov::vertexes(std::ostream& out, std::istream& in, graphTable& gt, List< std::string >&)
{
  std::string graphName;
  in >> graphName;
  if (!gt.contains(graphName))
  {
    throw std::logic_error("Invalid graph name");
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

void novikov::outbound(std::ostream& out, std::istream& in, graphTable& gt, List< std::string >&)
{
  std::string graphName, vertexName;
  in >> graphName >> vertexName;
  const Graph& graph = gt.at(graphName);
  if (!has(graph.vertexes, vertexName))
  {
    throw std::runtime_error("Invalid vertex");
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

void novikov::inbound(std::ostream& out, std::istream& in, graphTable& gt, List< std::string >&)
{
  std::string graphName, vertexName;
  in >> graphName >> vertexName;
  const Graph& graph = gt.at(graphName);
  if (!has(graph.vertexes, vertexName))
  {
    throw std::runtime_error("Invalid vertex");
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

void novikov::bind(std::ostream&, std::istream& in, graphTable& gt, List< std::string >&)
{
  std::string graphName;
  in >> graphName;
  Graph cpy = gt.at(graphName);
  std::string v1, v2;
  size_t w;
  in >> v1 >> v2 >> w;
  try
  {
    sortedInsert(cpy.connections[{v1, v2}], w);
  }
  catch (const std::runtime_error&)
  {
    cpy.connections.rehash(cpy.connections.size() * 2);
    sortedInsert(cpy.connections[{v1, v2}], w);
  }
  if (!has(cpy.pairs, {v1, v2}))
  {
    sortedInsert(cpy.pairs, {v1, v2});
  }
  if (!has(cpy.vertexes, v1))
  {
    sortedInsert(cpy.vertexes, v1);
  }
  if (!has(cpy.vertexes, v2))
  {
    sortedInsert(cpy.vertexes, v2);
  }
  std::swap(cpy, gt.at(graphName));
}

void novikov::cut(std::ostream&, std::istream& in, graphTable& gt, List< std::string >&)
{
  std::string graphName;
  in >> graphName;
  Graph cpy = gt.at(graphName);
  std::string v1, v2;
  size_t w;
  in >> v1 >> v2 >> w;
  List< size_t >& weights = cpy.connections.at({v1, v2});
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
    throw std::runtime_error("Invalid weight");
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

void novikov::create(std::ostream&, std::istream& in, graphTable& gt, List< std::string >& names)
{
  std::string graphName;
  in >> graphName;
  if (gt.contains(graphName))
  {
    throw std::runtime_error("Graph already exists");
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
      sortedInsert(graph.vertexes, vertex);
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
  sortedInsert(names, graphName);
}

void novikov::merge(std::ostream&, std::istream& in, graphTable& gt, List< std::string >& names)
{
  std::string graphName, grName1, grName2;
  in >> graphName >> grName1 >> grName2;
  if (gt.contains(graphName) || !gt.contains(grName1) || !gt.contains(grName2))
  {
    throw std::runtime_error("Invalid graph name");
  }

  Graph graph, gr1 = gt.at(grName1), gr2 = gt.at(grName2);
  graph.vertexes = gr1.vertexes;
  for (auto it = gr2.vertexes.begin(); it != gr2.vertexes.end(); ++it)
  {
    if (!has(graph.vertexes, *it))
    {
      sortedInsert(graph.vertexes, *it);
    }
  }

  graph.pairs = gr1.pairs;
  for (auto it = gr2.pairs.begin(); it != gr2.pairs.end(); ++it)
  {
    if (!has(graph.pairs, *it))
    {
      sortedInsert(graph.pairs, *it);
    }
  }

  for (auto it = gr1.connections.begin(); it != gr1.connections.end(); ++it)
  {
    List< size_t >* list = nullptr;
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
      sortedInsert(*list, *wIt);
    }
  }

  for (auto it = gr2.connections.begin(); it != gr2.connections.end(); ++it)
  {
    List< size_t >* list = nullptr;
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
      sortedInsert(*list, *wIt);
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
  sortedInsert(names, graphName);
}

void novikov::extract(std::ostream&, std::istream& in, graphTable& gt, List< std::string >& names)
{
  std::string newGraphName, oldGraphName;
  in >> newGraphName >> oldGraphName;
  if (gt.contains(newGraphName) || !gt.contains(oldGraphName))
  {
    throw std::runtime_error("Invalid graph name");
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
    sortedInsert(graph.vertexes, vertex);
  }

  for (auto it = oldGraph.pairs.begin(); it != oldGraph.pairs.end(); ++it)
  {
    if (has(graph.vertexes, (*it).first) && has(graph.vertexes, (*it).second))
    {
      sortedInsert(graph.pairs, (*it));
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
  sortedInsert(names, newGraphName);
}
