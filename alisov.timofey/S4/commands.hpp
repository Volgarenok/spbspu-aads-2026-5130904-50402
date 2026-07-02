#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <istream>
#include <ostream>
#include <string>
#include "BSTree.hpp"

namespace alisov
{
  using ds_t = BSTree< int, std::string, std::less< int > >;
  using datasets_t = BSTree< std::string, ds_t, std::less< std::string > >;

  void print(std::ostream &out, std::istream &in, datasets_t &datasets);
  void complement(std::ostream &out, std::istream &in, datasets_t &datasets);
  void intersect(std::ostream &out, std::istream &in, datasets_t &datasets);
  void union_(std::ostream &out, std::istream &in, datasets_t &datasets);
}
#endif
