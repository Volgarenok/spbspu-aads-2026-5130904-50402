#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <utility>

#include "../common/Vec.hpp"
#include "Hashtable.hpp"

namespace sogdanov {

  class Graph {
  public:
    Graph();

    void swap(Graph &other) noexcept;

    void bind(const std::string &u, const std::string &v, size_t w);
    void cut(const std::string &u, const std::string &v, size_t w);
    void add_vertex(const std::string &v);

    bool has_vertex(const std::string &v) const;
    Vector< std::string > get_vertices() const;

    HashTable< std::pair< std::string, std::string >, Vector< size_t > > &get_edges();

  private:
    Vector< std::string > vertices_;
    HashTable< std::pair< std::string, std::string >, Vector< size_t > > edges_;

    void internal_add_vertex(const std::string &v);
  };

}

#endif
