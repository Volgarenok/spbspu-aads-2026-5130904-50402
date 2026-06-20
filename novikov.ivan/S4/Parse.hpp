#ifndef PARSE_HPP
#define PARSE_HPP
#include <fstream>
#include <stdexcept>
#include <bilist.hpp>
#include "BSTree.hpp"

namespace novikov
{
  void parse(std::istream& in, List< BSTree< size_t, std::string, std::less< size_t > > >& trees);
}

#endif
