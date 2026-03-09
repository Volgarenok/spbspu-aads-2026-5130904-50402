#ifndef ACTIONS_HPP
#define ACTIONS_HPP
#include <iosfwd>
#include <utility>
#include "iters.hpp"
#include "list.hpp"

namespace dirko
{
  using l_pair_t = List< std::pair< std::string, List< long long > > >;
  using ci_pair_t = CIter< std::pair< std::string, List< long long > > >;
  void input(std::istream &is, l_pair_t &seq);
  void output(std::ostream &os, l_pair_t titles, List< List< long long > > list);
  List< List< long long > > process(l_pair_t seq);
  void printNames(std::ostream &os, l_pair_t names);
  void printList(std::ostream &os, List< List< long long > > list);
}

#endif
