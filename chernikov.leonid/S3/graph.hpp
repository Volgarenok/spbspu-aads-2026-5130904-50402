#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "hash_table.hpp"
#include "List.hpp"
#include <string>
#include <utility>
#include <algorithm>
#include <stdexcept>

namespace chernikov {

  class Graph
  {
  public:
    using Vertex = std::string;
    using Edge = std::pair< Vertex, Vertex >;
    using WeightList = List< int >;
    using VertexList = List< Vertex >;

  private:
    std::string name_;

    struct EdgeHash
    {
      std::size_t operator()(const Edge &e) const
      {
        std::size_t h1 = std::hash< std::string >()(e.first);
        std::size_t h2 = std::hash< std::string >()(e.second);
        return h1 ^ (h2 << 1);
      }
    };

    struct EdgeEqual
    {
      bool operator()(const Edge &a, const Edge &b) const
      {
        return a.first == b.first && a.second == b.second;
      }
    };

    HashTable< Edge, WeightList, EdgeHash, EdgeEqual > edges_;
    HashTable< Vertex, bool > vertices_;

  public:
    Graph():
      edges_(16)
    {
    }

    explicit Graph(const std::string &name):
      name_(name),
      edges_(16)
    {
    }

    const std::string &getName() const
    {
      return name_;
    }
    void setName(const std::string &name)
    {
      name_ = name;
    }

    void addVertex(const Vertex &vertex)
    {
      if (!vertices_.has(vertex))
      {
        try
        {
          vertices_.add(vertex, true);
        } catch (const std::overflow_error &)
        {
          vertices_.rehash(vertices_.bucket_count() * 2);
          vertices_.add(vertex, true);
        }
      }
    }

    void addEdge(const Vertex &from, const Vertex &to, int weight)
    {
      Edge key(from, to);

      addVertex(from);
      addVertex(to);

      if (!edges_.has(key))
      {
        try
        {
          edges_.add(key, WeightList());
        } catch (const std::overflow_error &)
        {
          edges_.rehash(edges_.bucket_count() * 2);
          edges_.add(key, WeightList());
        }
      }
      edges_.get(key).push_back(weight);
    }

    VertexList getVertices() const
    {
      VertexList result;

      for (auto it = vertices_.begin(); it != vertices_.end(); ++it)
      {
        result.push_back((*it).first);
      }

      sort_list(result);
      return result;
    }

    struct OutboundResult
    {
      Vertex vertex;
      WeightList weights;
    };

    List< OutboundResult > getOutbound(const Vertex &vertex) const
    {
      HashTable< Vertex, WeightList > out_edges;

      for (auto it = edges_.begin(); it != edges_.end(); ++it)
      {
        const Edge &edge = (*it).first;
        const WeightList &weights = (*it).second;

        if (edge.first == vertex)
        {
          if (!out_edges.has(edge.second))
          {
            out_edges.add(edge.second, WeightList());
          }
          WeightList &target = out_edges.get(edge.second);
          for (auto wit = weights.cbegin(); wit != weights.cend(); ++wit)
          {
            target.push_back(*wit);
          }
        }
      }

      List< OutboundResult > result;

      VertexList sorted_targets;
      for (auto it = out_edges.begin(); it != out_edges.end(); ++it)
      {
        sorted_targets.push_back((*it).first);
      }
      sort_list(sorted_targets);

      for (auto it = sorted_targets.cbegin(); it != sorted_targets.cend(); ++it)
      {
        WeightList &wl = out_edges.get(*it);
        sort_list(wl);
        result.push_back({*it, wl});
      }

      return result;
    }

    List< OutboundResult > getInbound(const Vertex &vertex) const
    {
      HashTable< Vertex, WeightList > in_edges;

      for (auto it = edges_.begin(); it != edges_.end(); ++it)
      {
        const Edge &edge = (*it).first;
        const WeightList &weights = (*it).second;

        if (edge.second == vertex)
        {
          if (!in_edges.has(edge.first))
          {
            in_edges.add(edge.first, WeightList());
          }
          WeightList &source = in_edges.get(edge.first);
          for (auto wit = weights.cbegin(); wit != weights.cend(); ++wit)
          {
            source.push_back(*wit);
          }
        }
      }

      List< OutboundResult > result;

      VertexList sorted_sources;
      for (auto it = in_edges.begin(); it != in_edges.end(); ++it)
      {
        sorted_sources.push_back((*it).first);
      }
      sort_list(sorted_sources);

      for (auto it = sorted_sources.cbegin(); it != sorted_sources.cend(); ++it)
      {
        WeightList &wl = in_edges.get(*it);
        sort_list(wl);
        result.push_back({*it, wl});
      }

      return result;
    }

    bool hasVertex(const Vertex &vertex) const
    {
      return vertices_.has(vertex);
    }

    bool removeEdge(const Vertex &from, const Vertex &to, int weight)
    {
      Edge key(from, to);

      if (!edges_.has(key))
      {
        return false;
      }

      WeightList &weights = edges_.get(key);
      WeightList new_weights;
      bool found = false;

      for (auto it = weights.begin(); it != weights.end(); ++it)
      {
        if (!found && *it == weight)
        {
          found = true;
        } else
        {
          new_weights.push_back(*it);
        }
      }

      if (found)
      {
        weights = std::move(new_weights);
        if (weights.empty())
        {
          edges_.drop(key);
        }
      }

      return found;
    }

    Graph extractSubgraph(const VertexList &sub_vertices) const
    {
      Graph subgraph;

      for (auto it = sub_vertices.cbegin(); it != sub_vertices.cend(); ++it)
      {
        subgraph.addVertex(*it);
      }

      for (auto it = edges_.begin(); it != edges_.end(); ++it)
      {
        const Edge &edge = (*it).first;
        const WeightList &weights = (*it).second;

        if (subgraph.hasVertex(edge.first) && subgraph.hasVertex(edge.second))
        {
          for (auto wit = weights.cbegin(); wit != weights.cend(); ++wit)
          {
            subgraph.addEdge(edge.first, edge.second, *wit);
          }
        }
      }

      return subgraph;
    }

    static Graph merge(const Graph &g1, const Graph &g2, const std::string &new_name)
    {
      Graph merged(new_name);

      for (auto it = g1.edges_.begin(); it != g1.edges_.end(); ++it)
      {
        const Edge &edge = (*it).first;
        const WeightList &weights = (*it).second;
        for (auto wit = weights.cbegin(); wit != weights.cend(); ++wit)
        {
          merged.addEdge(edge.first, edge.second, *wit);
        }
      }

      for (auto it = g2.edges_.begin(); it != g2.edges_.end(); ++it)
      {
        const Edge &edge = (*it).first;
        const WeightList &weights = (*it).second;
        for (auto wit = weights.cbegin(); wit != weights.cend(); ++wit)
        {
          merged.addEdge(edge.first, edge.second, *wit);
        }
      }

      return merged;
    }

    size_t edgeCount() const
    {
      return edges_.size();
    }

  private:
    template < typename T > static void sort_list(List< T > &lst)
    {
      if (lst.size() <= 1)
        return;

      for (auto it1 = lst.begin(); it1 != lst.end(); ++it1)
      {
        for (auto it2 = it1; it2 != lst.end(); ++it2)
        {
          if (*it2 < *it1)
          {
            std::swap(*it1, *it2);
          }
        }
      }
    }
  };

}

#endif
