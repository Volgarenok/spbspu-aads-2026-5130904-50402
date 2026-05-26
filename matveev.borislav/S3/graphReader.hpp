#ifndef MATVEEV_GRAPH_READER_HPP
#define MATVEEV_GRAPH_READER_HPP

#include "graphCollection.hpp"

#include <iosfwd>

namespace matveev
{
void readGraphs(std::istream& input, GraphCollection& graphs);
}

#endif
