#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <string>
#include "BiTree/BiTree.hpp"

namespace afanasev
{
  using Dataset = BSTree< int, std::string >;
  using Datasets = BSTree< std::string, Dataset >;

  void cmdPrint(std::istream & in, std::ostream & out, Datasets & ds);
  void cmdComplement(std::istream & in, std::ostream & out, Datasets & ds);
  void cmdIntersect(std::istream & in, std::ostream & out, Datasets & ds);
  void cmdUnion(std::istream & in, std::ostream & out, Datasets & ds);
}

#endif
