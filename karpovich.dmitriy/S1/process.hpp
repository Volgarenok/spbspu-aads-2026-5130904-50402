#ifndef UTILS_HPP
#define UTILS_HPP
#include <iostream>
#include "list.hpp"

namespace karpovich
{
  using list_pair_t = List< std::pair< std::string, List< size_t > > >;
  void input(std::istream& in, list_pair_t& list);
  void output(std::ostream& out, const list_pair_t& names, const List<List<size_t>>& transposed);
  void transpose(const list_pair_t& list, List< List< size_t > >& data);
}
#endif
