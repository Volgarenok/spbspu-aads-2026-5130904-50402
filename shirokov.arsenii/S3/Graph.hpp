#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <bilist.hpp>
#include <functional>
#include <string>
#include <utility>
#include "HashTable.hpp"
#include "hasher.hpp"

namespace shirokov
{
  using pairOfVertexes = std::pair< std::string, std::string >;
  struct Graph
  {
    shirokov::HashTable< pairOfVertexes, shirokov::BiList< size_t >, shirokov::SHA1< pairOfVertexes >,
        std::equal_to< pairOfVertexes > >
        vertexes;
  };
}

#endif
