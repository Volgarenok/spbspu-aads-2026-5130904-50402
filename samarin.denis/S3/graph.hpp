#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <utility>
#include "hash_table.hpp"
#include "hashers.hpp"
#include <list.hpp>

namespace samarin {

  class Graph {
  public:
    using Weight = unsigned long;
    using WeightList = List< Weight >;
    using EdgeKey = std::pair< std::string, std::string >;
    using VertexTable = HashTable< std::string, bool, StringHash >;
    using EdgeTable = HashTable< EdgeKey, WeightList, PairHash >;

    bool hasVertex(const std::string& name) const;
    void addVertex(const std::string& name);
    void bind(const std::string& from, const std::string& to, Weight weight);
    bool cut(const std::string& from, const std::string& to, Weight weight);
    void absorb(const Graph& other);
    void copyEdgesWithin(const Graph& source);

    const VertexTable& vertices() const;
    const EdgeTable& edges() const;

  private:
    VertexTable vertices_;
    EdgeTable edges_;
  };

}

#endif
