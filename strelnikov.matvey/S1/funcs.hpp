#ifndef FUNCS_HPP
#define FUNCS_HPP

#include "iters.hpp"
#include "list.hpp"
#include "node.hpp"
#include <iostream>

namespace strelnikov
{
  void input(std::istream &, List< std::pair< std::string, List< size_t > > > &);
  template < class T > size_t size(const List< T > &);
  List< List< size_t > > leave_nums(List< std::pair< std::string, List< size_t > > > &);
  List< size_t > get_sum(List< List< size_t > > &);
  std::ostream &printNames(std::ostream &, const List< std::pair< std::string, List< size_t > > > &);
  std::ostream &printList(std::ostream &, const List< List< size_t > > &);
  std::ostream &printSum(std::ostream &, const List< size_t > &);
}

#endif