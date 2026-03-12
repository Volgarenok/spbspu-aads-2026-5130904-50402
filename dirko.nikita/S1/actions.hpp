#ifndef ACTIONS_HPP
#define ACTIONS_HPP
#include <iosfwd>
#include <utility>
#include "iters.hpp"
#include "list.hpp"

namespace dirko
{
  using l_pair_t = List< std::pair< std::string, List< size_t > > >;
  using ci_pair_t = CIter< std::pair< std::string, List< size_t > > >;
  void input(std::istream &is, l_pair_t &seq);
  std::ostream &printSum(std::ostream &os, const List< List< size_t > > &list);
  List< List< size_t > > process(const l_pair_t &seq);
  std::ostream &printNames(std::ostream &os, const l_pair_t &names);
  std::ostream &printList(std::ostream &os, const List< List< size_t > > &list);
}

#endif
