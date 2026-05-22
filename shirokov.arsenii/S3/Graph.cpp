#include "Graph.hpp"
#include <stdexcept>
#include "bilist.hpp"

void shirokov::graphs(std::ostream& out, std::istream&, graphTable&, BiList< std::string >& names)
{
  for (auto it = names.begin(); it != names.end(); ++it)
  {
    out << *it << '\n';
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
  for (auto it = gt[graphName].vertexes.begin(); it != gt[graphName].vertexes.end(); ++it)
  {
    out << *it << '\n';
  }
}

void shirokov::outbound(std::ostream& out, std::istream& in, graphTable& gt, BiList< std::string >&)
{
  std::string graphName, vertexName;
  in >> graphName >> vertexName;
  const Graph& graph = gt.at(graphName);
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
