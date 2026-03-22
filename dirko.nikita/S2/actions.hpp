#ifndef ACTIONS_HPP
#define ACTIONS_HPP
#include <iosfwd>
#include <string>
#include "queue.hpp"

namespace dirko
{
  Queue< Queue< std::string > > input(std::istream &is);
}

#endif
