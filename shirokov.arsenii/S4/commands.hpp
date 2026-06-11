#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <string>
#include "BSTree.hpp"

namespace shirokov
{
  using datasets_t = BSTree< std::string, BSTree< int, std::string, std::less< int > >, std::less< std::string > >;

  void print(std::ostream&, std::istream&, shirokov::datasets_t&);
  void complement(std::ostream&, std::istream&, shirokov::datasets_t&);
  void intersect(std::ostream&, std::istream&, shirokov::datasets_t&);
  void union_(std::ostream&, std::istream&, shirokov::datasets_t&);
}

#endif
