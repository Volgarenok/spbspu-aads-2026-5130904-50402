#ifndef CMD_HPP
#define CMD_HPP

#include <iostream>
#include <string>
#include "bstree.hpp"

namespace sogdanov
{

  void cmdPrint(std::istream &in, std::ostream &out, BSTree<std::string, BSTree<int, std::string>> &datasets);
  void cmdComplement(std::istream &in, std::ostream &out, BSTree<std::string, BSTree<int, std::string>> &datasets);
  void cmdIntersect(std::istream &in, std::ostream &out, BSTree<std::string, BSTree<int, std::string>> &datasets);
  void cmdUnion(std::istream &in, std::ostream &out, BSTree<std::string, BSTree<int, std::string>> &datasets);

}

#endif
