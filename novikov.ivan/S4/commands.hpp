#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <istream>
#include <ostream>
#include <string>
#include <bilist.hpp>
#include "BSTree.hpp"

namespace novikov
{
  using BSTList = novikov::List< novikov::BSTree< size_t, std::string, std::less< size_t > > >;

  void print(std::istream& in, std::ostream& out, BSTList& bstl);
  void complement(std::istream& in, std::ostream&, BSTList& bstl);
  void intersect(std::istream& in, std::ostream&, BSTList& bstl);
  void union_(std::istream& in, std::ostream&, BSTList& bstl);
}

#endif
