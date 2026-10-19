#ifndef TASK_HPP
#define TASK_HPP
#include <iostream>
#include "list.hpp"

namespace permyakov
{
  using list_t = List< std::pair< std::string, List< size_t > > >;

  bool input(std::istream & in, list_t & list);
  void inputNum(std::istream & in, List< size_t > & numbers);
  List< size_t > idList(const list_t list, size_t id);
  size_t sumList(const List< size_t >);
  void outputName(std::ostream & out, const list_t & lsit);
  void outputNum(std::ostream & out, const List< size_t > & lsit);
}

#endif
