#ifndef GRAPH_COLLECTION_HPP
#define GRAPH_COLLECTION_HPP

#include <string>
#include "graph.hpp"
#include "hash_table.hpp"
#include "hashers.hpp"

namespace samarin {

  using GraphCollection = HashTable< std::string, Graph, StringHash >;

}

#endif
