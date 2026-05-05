#ifndef FUNCS_HPP
#define FUNCS_HPP

#include <iostream>
#include "../common/iters.hpp"
#include "../common/list.hpp"
#include "../common/node.hpp"

namespace strelnikov
{
  List< std::pair< std::string, List< size_t > > > input(std::istream &in);
  template < class T >
  size_t size(const List< T > &list)
  {
    size_t res = 0;
    auto it = list.cbegin();
    while (it != list.cend()) {
      res++;
      it++;
    }

    return res;
  }
  List< List< size_t > > leave_nums(List< std::pair< std::string, List< size_t > > > &);
  List< size_t > get_sum(List< List< size_t > > &);
  std::ostream &printNames(std::ostream &, const List< std::pair< std::string, List< size_t > > > &);
  std::ostream &printList(std::ostream &, const List< List< size_t > > &);
  std::ostream &printSum(std::ostream &, const List< size_t > &);
}

#endif
