#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iosfwd>
#include "dataset.hpp"

namespace samarin {

  void processCommands(std::istream & in, std::ostream & out, DatasetCollection & datasets);

}

#endif
