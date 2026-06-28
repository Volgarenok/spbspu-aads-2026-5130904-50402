#ifndef INPUT_HPP
#define INPUT_HPP

#include <iosfwd>
#include "graph_collection.hpp"

namespace samarin {

  void readGraphs(std::istream& in, GraphCollection& graphs);

}

#endif
