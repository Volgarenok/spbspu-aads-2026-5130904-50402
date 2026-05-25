#ifndef LIST_IO_HPP
#define LIST_IO_HPP

#include <iostream>
#include <string>
#include <utility>
#include "list.hpp"

namespace afanasev
{
  bool input(std::istream & in, List< std::pair< std::string, List< size_t > > > & list);
  void output(std::ostream & out, List< std::pair< std::string, List< size_t > > > & list);
}

#endif
