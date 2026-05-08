#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "../common/Vec.hpp"
#include "Hashtable.hpp"
#include <string>
#include <utility>

namespace sogdanov
{
  class Graph
  {
    Vector<std::string> vertices_;
    HashTable<std::pair<std::string, std::string>, Vector<unsigned int>> edges_;

    void internal_add_vertex(const std::string &v);

  public:
    Graph();

    void swap(Graph &other) noexcept;

    void bind(const std::string &u, const std::string &v, unsigned int w);
    void cut(const std::string &u, const std::string &v, unsigned int w);
    void add_vertex(const std::string &v);

    bool has_vertex(const std::string &v) const;
    Vector<std::string> get_vertices() const;

    HashTable<std::pair<std::string, std::string>, Vector<unsigned int>> &get_edges();
  };

}
#endif