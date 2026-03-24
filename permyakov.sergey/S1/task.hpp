#ifndef TASK_HPP
#define TASK_HPP

#include <iostream>
#include "list.hpp"

namespace permyakov
{
  using list_t = List < std::pair < std::string, List < size_t > > >;

  void input(std::istream &in, list_t & list);
  //void output(std::ostream &out, const list_t & list);
}

#endif