#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iosfwd>
#include "graph_collection.hpp"

namespace samarin {

  void processCommands(std::istream& in, std::ostream& out, GraphCollection& graphs);

}

#endif
