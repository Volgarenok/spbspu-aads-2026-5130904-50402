#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <cstddef>
#include <string>
#include "../common/list.hpp"
#include "hash-table.hpp"
#include "hasher.hpp"
namespace dirko
{
  class Graph
  {
  public:
    using key_t = std::pair< std::string, std::string >;
    List< std::string > vertices;
    HashTable< key_t, List< size_t >, PairSipHasher< std::string >, std::equal_to< key_t > > conections;
    Graph() noexcept;
    explicit Graph(size_t slots);
    void addVertex(const std::string &vertex);
    void removeVertex(const std::string &vertex);
    void addConection(const std::string &from, const std::string &to, size_t weight);
    void removeConection(const std::string &from, const std::string &to, size_t weight);
  };

}
#endif
