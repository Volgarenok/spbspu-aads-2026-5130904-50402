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

  if (verts.getSize() == 0)
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
}
void afanasev::cmdInbound(std::istream & in, std::ostream & out, GraphSet & graphs)
{
}
void afanasev::cmdBind(std::istream & in, std::ostream & out, GraphSet & graphs)
{
}
void afanasev::cmdCut(std::istream & in, std::ostream & out, GraphSet & graphs)
{
}
void afanasev::cmdCreate(std::istream & in, std::ostream & out, GraphSet & graphs)
{
}
void afanasev::cmdMerge(std::istream & in, std::ostream & out, GraphSet & graphs)
{
}
void afanasev::cmdExtract(std::istream & in, std::ostream & out, GraphSet & graphs)
{
}

#endif