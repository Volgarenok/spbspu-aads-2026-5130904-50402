#include "Graph.hpp"
#include <stdexcept>

namespace sogdanov
{

  Graph::Graph() : edges_(100) {}

  void Graph::internal_add_vertex(const std::string &v)
  {
    bool exists = false;
    for (size_t i = 0; i < vertices_.getSize(); ++i)
    {
      if (vertices_[i] == v)
      {
        exists = true;
        break;
      }
    }
    if (!exists)
      vertices_.pushBack(v);
  }

  void Graph::swap(Graph &other) noexcept
  {
    vertices_.swap(other.vertices_);
    edges_.swap(other.edges_);
  }

  void Graph::bind(const std::string &u, const std::string &v, size_t w)
  {
    Graph tmp = *this;
    tmp.internal_add_vertex(u);
    tmp.internal_add_vertex(v);

    std::pair<std::string, std::string> edge_key{u, v};
    if (!tmp.edges_.has(edge_key))
    {
      Vector<size_t> weights;
      weights.pushBack(w);
      tmp.edges_.add(edge_key, weights);
    }
    else
    {
      tmp.edges_.get(edge_key).pushBack(w);
    }

    swap(tmp);
  }

  void Graph::cut(const std::string &u, const std::string &v, size_t w)
  {
    Graph tmp = *this;
    std::pair<std::string, std::string> edge_key{u, v};

    if (!tmp.edges_.has(edge_key))
      throw std::logic_error("Edge not found");

    auto &weights = tmp.edges_.get(edge_key);
    bool found = false;
    for (size_t i = 0; i < weights.getSize(); ++i)
    {
      if (weights[i] == w)
      {
        for (size_t j = i; j < weights.getSize() - 1; ++j)
        {
          weights[j] = weights[j + 1];
        }
        weights.popBack();
        found = true;
        break;
      }
    }
    if (!found)
      throw std::logic_error("Weight not found");

    if (weights.isEmpty())
    {
      tmp.edges_.drop(edge_key);
    }

    swap(tmp);
  }

  void Graph::add_vertex(const std::string &v)
  {
    Graph tmp = *this;
    tmp.internal_add_vertex(v);
    swap(tmp);
  }

  bool Graph::has_vertex(const std::string &v) const
  {
    for (size_t i = 0; i < vertices_.getSize(); ++i)
    {
      if (vertices_[i] == v)
        return true;
    }
    return false;
  }

  Vector<std::string> Graph::get_vertices() const {
    return vertices_;
  }

  HashTable<std::pair<std::string, std::string>, Vector<size_t>> &Graph::get_edges() {
    return edges_;
  }

}
