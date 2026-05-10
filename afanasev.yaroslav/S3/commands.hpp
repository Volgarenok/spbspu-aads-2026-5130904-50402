#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <string>
#include "HashTable/HashTable.hpp"
#include "HashTable/HashFunction.hpp"
#include "Graph.hpp"

namespace afanasev
{
  using GraphSet = HashTable< std::string, Graph, Hasher< std::string >, std::equal_to< std::string > >;

  void cmdGraphs(std::istream & in, std::ostream & out, GraphSet & graphs);
  void cmdVertexes(std::istream & in, std::ostream & out, GraphSet & graphs);
  void cmdOutbound(std::istream & in, std::ostream & out, GraphSet & graphs);
  void cmdInbound(std::istream & in, std::ostream & out, GraphSet & graphs);
  void cmdBind(std::istream & in, std::ostream & out, GraphSet & graphs);
  void cmdCut(std::istream & in, std::ostream & out, GraphSet & graphs);
  void cmdCreate(std::istream & in, std::ostream & out, GraphSet & graphs);
  void cmdMerge(std::istream & in, std::ostream & out, GraphSet & graphs);
  void cmdExtract(std::istream & in, std::ostream & out, GraphSet & graphs);

  namespace detail
  {
    void sortStrings(Vector< std::string > & vec)
    {
      for (size_t i = 1; i < vec.getSize(); ++i)
      {
        std::string key = vec[i];
        size_t j = i;
        while (j > 0 && vec[j - 1] > key)
        {
          vec[j] = vec[j - 1];
          --j;
        }
        vec[j] = key;
      }
    }

    void sortWeights(Vector< size_t > & vec)
    {
      for (size_t i = 1; i < vec.getSize(); ++i)
      {
        size_t key = vec[i];
        size_t j = i;
        while (j > 0 && vec[j - 1] > key)
        {
          vec[j] = vec[j - 1];
          --j;
        }
        vec[j] = key;
      }
    }
  }
}

void afanasev::cmdGraphs(std::istream &, std::ostream & out, GraphSet & graphs)
{
  Vector< std::string > names;

  for (GraphSet::HIter it = graphs.begin(); it != graphs.end(); ++it)
  {
    names.pushBack((*it).first);
  }

  detail::sortStrings(names);

  if (!names.getSize())
  {
    out << "\n";
  }
  else
  {
    for (size_t i = 0; i < names.getSize(); ++i)
    {
      out << names[i] << "\n";
    }
  }
}

void afanasev::cmdVertexes(std::istream & in, std::ostream & out, GraphSet & graphs)
{
  std::string g_name;
  in >> g_name;

  if (!graphs.has(g_name))
  {
    throw std::runtime_error("No graph");
  }

  const Graph & g = graphs.get(g_name);
  Vector< std::string > verts;
  const std::set< std::string > & vertices = g.getVertices();

  for (std::set< std::string >::const_iterator it = vertices.begin(); it != vertices.end(); ++it)
  {
    verts.pushBack(*it);
  }

  detail::sortStrings(verts);

  if (!verts.getSize())
  {
    out << "\n";
  }
  else
  {
    for (size_t i = 0; i < verts.getSize(); ++i)
    {
      out << verts[i] << "\n";
    }
  }
}

void afanasev::cmdOutbound(std::istream & in, std::ostream & out, GraphSet & graphs)
{
  std::string g_name, v_name;
  in >> g_name >> v_name;
  Graph & g = graphs.get(g_name);
  if (!g.hasVertex(v_name))
  {
    throw std::out_of_range("Vertex missing");
  }

  Vector< std::pair< std::string, int > > edges = g.getOutEdges(v_name);
  Vector< std::pair< std::string, size_t > > sortedEdges;
  for (size_t i = 0; i < edges.getSize(); ++i)
  {
    sortedEdges.pushBack(std::make_pair(edges[i].first, static_cast< size_t >(edges[i].second)));
  }

  for (size_t i = 1; i < sortedEdges.getSize(); ++i)
  {
    std::pair< std::string, size_t > key = sortedEdges[i];
    size_t j = i;
    while ((j > 0) && ((sortedEdges[j - 1].first > key.first) ||
           ((sortedEdges[j - 1].first == key.first) && (sortedEdges[j - 1].second > key.second))))
    {
      sortedEdges[j] = sortedEdges[j - 1];
      --j;
    }
    sortedEdges[j] = key;
  }

  if (!sortedEdges.getSize())
  {
    out << "\n";
  }
  else
  {
    size_t idx = 0;
    while (idx < sortedEdges.getSize())
    {
      const std::string & currentDest = sortedEdges[idx].first;
      out << currentDest;
      while ((idx < sortedEdges.getSize()) && (sortedEdges[idx].first == currentDest))
      {
        out << " " << sortedEdges[idx].second;
        ++idx;
      }
      out << "\n";
    }
  }
}

void afanasev::cmdInbound(std::istream & in, std::ostream & out, GraphSet & graphs)
{
  std::string g_name, v_name;
  in >> g_name >> v_name;
  Graph & g = graphs.get(g_name);
  if (!g.hasVertex(v_name))
  {
    throw std::out_of_range("Vertex missing");
  }

  Vector< std::pair< std::string, int > > edges = g.getInEdges(v_name);
  Vector< std::pair< std::string, size_t > > sortedEdges;
  for (size_t i = 0; i < edges.getSize(); ++i)
  {
    sortedEdges.pushBack(std::make_pair(edges[i].first, static_cast< size_t >(edges[i].second)));
  }

  for (size_t i = 1; i < sortedEdges.getSize(); ++i)
  {
    std::pair< std::string, size_t > key = sortedEdges[i];
    size_t j = i;
    while ((j > 0) && ((sortedEdges[j - 1].first > key.first) ||
           ((sortedEdges[j - 1].first == key.first) && (sortedEdges[j - 1].second > key.second))))
    {
      sortedEdges[j] = sortedEdges[j - 1];
      --j;
    }
    sortedEdges[j] = key;
  }

  if (!sortedEdges.getSize())
  {
    out << "\n";
  }
  else
  {
    size_t idx = 0;
    while (idx < sortedEdges.getSize())
    {
      const std::string & currentSrc = sortedEdges[idx].first;
      out << currentSrc;
      while ((idx < sortedEdges.getSize()) && (sortedEdges[idx].first == currentSrc))
      {
        out << " " << sortedEdges[idx].second;
        ++idx;
      }
      out << "\n";
    }
  }
}

void afanasev::cmdBind(std::istream & in, std::ostream &, GraphSet & graphs)
{
  std::string g_name, v1, v2;
  size_t w = 0;
  in >> g_name >> v1 >> v2 >> w;
  Graph & g = graphs.get(g_name);
  g.addEdge(v1, v2, static_cast< int >(w));
}

void afanasev::cmdCut(std::istream & in, std::ostream &, GraphSet & graphs)
{
  std::string g_name, v1, v2;
  size_t w = 0;
  in >> g_name >> v1 >> v2 >> w;
  Graph & g = graphs.get(g_name);

  if (!g.hasVertex(v1) || !g.hasVertex(v2))
  {
    throw std::runtime_error("Vertex missing");
  }

  g.removeEdge(v1, v2, static_cast< int >(w));
}

void afanasev::cmdCreate(std::istream & in, std::ostream &, GraphSet & graphs)
{
  std::string g_name;
  in >> g_name;
  if (!in)
  {
    throw std::runtime_error("Invalid input");
  }
  if (graphs.has(g_name))
  {
    throw std::runtime_error("Graph already exists");
  }

  size_t k = 0;
  in >> k;
  if (!in)
  {
    throw std::runtime_error("Invalid vertex count");
  }

  Graph g;
  for (size_t i = 0; i < k; ++i)
  {
    std::string v;
    in >> v;
    if (!in)
    {
      throw std::runtime_error("Invalid vertex name");
    }
    g.addVertex(v);
  }

  graphs.add(g_name, std::move(g));
}

void afanasev::cmdMerge(std::istream & in, std::ostream & out, GraphSet & graphs)
{
}

void afanasev::cmdExtract(std::istream & in, std::ostream & out, GraphSet & graphs)
{
}

#endif