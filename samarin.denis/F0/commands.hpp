#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iosfwd>
#include "documents.hpp"

namespace samarin {

  void processCommands(std::istream& in, std::ostream& out, Documents& docs);

}

#endif
