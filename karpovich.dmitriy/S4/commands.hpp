#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <string>
#include "bstree.hpp"

namespace karpovich
{
  using Dataset = BSTree< int, std::string >;
  using Datasets = BSTree< std::string, Dataset >;

  void cmdPrint(std::istream &in, std::ostream &out, Datasets &datasets);
  void cmdComplement(std::istream &in, std::ostream &out, Datasets &datasets);
  void cmdIntersect(std::istream &in, std::ostream &out, Datasets &datasets);
  void cmdUnion(std::istream &in, std::ostream &out, Datasets &datasets);
}

#endif
