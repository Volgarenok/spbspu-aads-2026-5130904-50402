#ifndef ACTIONS_HPP
#define ACTIONS_HPP
#include <iosfwd>
#include <utility>
#include "list.hpp"

namespace dirko
{
  using l_pair_t = List< std::pair< std::string, List< long long > > >;
  void input(std::istream &is, l_pair_t &lst);
}

#endif
