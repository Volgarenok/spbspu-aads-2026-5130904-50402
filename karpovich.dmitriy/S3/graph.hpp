#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <cstddef>
#include <stdexcept>
#include <string>
#include <utility>
#include "../common/Vector.hpp"
#include "../common/list.hpp"
#include "hashFunctions.hpp"
#include "hashIters.hpp"
#include "hashTable.hpp"

namespace karpovich
{
  struct Graph
  {
    using str_t = std::pair< std::string, std::string >;
    List< std::string > vertices;
    HashTable< str_t, List< size_t >, PairHasher< std::string >, std::equal_to< str_t > > edges;
    Graph() noexcept;
    explicit Graph(size_t edgeSlots);

    void addVertex(const std::string &v);
    void removeVertex(const std::string &v);
    void addEdge(const std::string &from, const std::string &to, size_t weight);
    void removeEdge(const std::string &from, const std::string &to, size_t weight);
  };
}

#endif
