#ifndef MATVEEV_GAMEWORLD_COMMAND_HANDLER_HPP
#define MATVEEV_GAMEWORLD_COMMAND_HANDLER_HPP

#include "world.hpp"

#include "../common/list.hpp"

#include <iosfwd>
#include <string>

namespace matveev
{
bool executeCommand(std::ostream& out, World& world, const List< std::string >& tokens);
}

#endif
