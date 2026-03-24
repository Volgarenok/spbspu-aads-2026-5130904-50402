#ifndef TASK_HPP
#define TASK_HPP

#include <iostream>
#include "list.hpp"

namespace permyakov
{
  using pair_t = List< std::pair< std::string, List< size_t > > >;

  void input(std::istream &in, pair_t & list);
  void output(std::ostream &out, const pair_t & list);
}

#endif