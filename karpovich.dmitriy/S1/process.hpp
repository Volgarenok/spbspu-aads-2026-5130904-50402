#ifndef UTILS_HPP
#define UTILS_HPP
#include <iostream>
#include "list.hpp"

namespace karpovich
{
  void input(std::istream& in, List< std::pair< std::string, List< size_t > > >& list);
  void output(std::ostream& out);
}
#endif
