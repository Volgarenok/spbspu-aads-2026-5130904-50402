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

void afanasev::cmdMerge(std::istream & in, std::ostream &, GraphSet & graphs)
{
  std::string new_g, g1, g2;
  in >> new_g >> g1 >> g2;
  if (graphs.has(new_g) || !graphs.has(g1) || !graphs.has(g2))
  {
    throw std::runtime_error("Invalid");
  }
  Graph merged;
  const Graph & src1 = graphs.get(g1);
  const Graph & src2 = graphs.get(g2);

  const std::set< std::string > & verts1 = src1.getVertices();
  for (std::set< std::string >::const_iterator it = verts1.begin(); it != verts1.end(); ++it)
  {
    merged.addVertex(*it);
  }
  const std::set< std::string > & verts2 = src2.getVertices();
  for (std::set< std::string >::const_iterator it = verts2.begin(); it != verts2.end(); ++it)
  {
    merged.addVertex(*it);
  }

  for (std::set< std::string >::const_iterator it = verts1.begin(); it != verts1.end(); ++it)
  {
    const std::string & v = *it;
    Vector< std::pair< std::string, int > > edges = src1.getOutEdges(v);
    for (size_t i = 0; i < edges.getSize(); ++i)
    {
      merged.addEdge(v, edges[i].first, edges[i].second);
    }
  }
  for (std::set< std::string >::const_iterator it = verts2.begin(); it != verts2.end(); ++it)
  {
    const std::string & v = *it;
    Vector< std::pair< std::string, int > > edges = src2.getOutEdges(v);
    for (size_t i = 0; i < edges.getSize(); ++i)
    {
      merged.addEdge(v, edges[i].first, edges[i].second);
    }
  }
  graphs.add(new_g, std::move(merged));
}

void afanasev::cmdExtract(std::istream & in, std::ostream &, GraphSet & graphs)
{
  std::string new_g, old_g;
  size_t k = 0;
  in >> new_g >> old_g >> k;
  if (graphs.has(new_g) || !graphs.has(old_g))
  {
    throw std::runtime_error("Invalid");
  }
  const Graph & src = graphs.get(old_g);
  List< std::string > required;
  for (size_t i = 0; i < k; ++i)
  {
    std::string v;
    in >> v;
    required.pushFront(v);
  }

  for (LIter< std::string > it = required.begin(); it != LIter< std::string >(); ++it)
  {
    if (!src.hasVertex(*it))
    {
      throw std::out_of_range("Vertex missing");
    }
  }

  Graph ext;
  for (LIter< std::string > it = required.begin(); it != LIter< std::string >(); ++it)
  {
    ext.addVertex(*it);
  }

  for (LIter< std::string > it = required.begin(); it != LIter< std::string >(); ++it)
  {
    const std::string & from = *it;
    Vector< std::pair< std::string, int > > edges = src.getOutEdges(from);
    for (size_t i = 0; i < edges.getSize(); ++i)
    {
      const std::string & to = edges[i].first;
      bool toOk = false;
      for (LIter< std::string > rit = required.begin(); rit != LIter< std::string >(); ++rit)
      {
        if (*rit == to)
        {
          toOk = true;
          break;
        }
      }
      if (toOk)
      {
        ext.addEdge(from, to, edges[i].second);
      }
    }
  }
  graphs.add(new_g, std::move(ext));
}

#endif
