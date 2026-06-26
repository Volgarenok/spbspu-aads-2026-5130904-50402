#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <cstddef>
#include <string>
#include <functional>
#include <stdexcept>
#include <utility>
#include "hashTable.hpp"
#include "hasher.hpp"
#include <list.hpp>

namespace muhamadiarov
{
  struct PairHash
  {
    size_t operator()(const std::pair<std::string, std::string>& p) const
    {
      SipHash<std::string> hasher;
      return hasher(p.first) ^ (hasher(p.second) << 1);
    }
  };
  struct Graph
  {
    using key_t = std::pair< std::string, std::string >;
    List< std::string > vertices_;
    HashTable< key_t, List< size_t >, PairHash, std::equal_to< key_t > > bonds_;
    Graph();
    explicit Graph(size_t bucketCapacity, size_t bucketCounts);
    bool findV(const std::string& vertex) const;
    bool findC(const std::string& from, const std::string& to, size_t weight);
    void addVertex(const std::string& vertex);
    void removeVertex(const std::string& vertex);
    void addConnection(const std::string& from, const std::string& to, size_t weight);
    void removeConnection(const std::string& from, const std::string& to, size_t weight);
  };
}
#endif
