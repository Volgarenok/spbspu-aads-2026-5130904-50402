#ifndef FUNCTION_HPP
#define FUNCTION_HPP
#include <iosfwd>
#include <utility>
#include "list.hpp"
#include "lciter.hpp"
#include "liter.hpp"

namespace muhamadiarov
{
  using l_pair_str_l_t = List< std::pair< std::string, List< size_t > > >;
  void input(std::istream& in, l_pair_str_l_t& list); 
}
#endif
