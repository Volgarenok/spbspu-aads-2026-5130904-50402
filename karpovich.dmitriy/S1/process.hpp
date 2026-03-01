#ifndef UTILS_HPP
#define UTILS_HPP
#include <iostream>
#include "list.hpp"

namespace karpovich
{
  using list_pair_t = list_pair_t;
  void input(std::istream& in, list_pair_t& list);
  void output(std::ostream& out);
  void transpose(const list_pair_t& list, list_pair_t& data);
}
#endif
