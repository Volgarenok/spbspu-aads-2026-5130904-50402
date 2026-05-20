#ifndef MATVEEV_COMMAND_HANDLER_HPP
#define MATVEEV_COMMAND_HANDLER_HPP

#include "commands.hpp"
#include "graphCollection.hpp"
#include "parserUtils.hpp"

#include <iosfwd>
#include <string>

namespace matveev
{
bool executeCommand(std::ostream& out, GraphCollection& graphs, const List< std::string >& tokens);
}

#endif
