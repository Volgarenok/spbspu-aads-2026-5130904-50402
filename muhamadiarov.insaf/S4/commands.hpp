#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iostream>
#include <string>
#include "bstree.hpp"

namespace muhamadiarov
{
  using Dataset_t = BSTree< int, std::string >;
  using ManyDatasets_t = BSTree< std::string, Dataset_t >;

  bool isInt(const std::string& str);
  void cmdPrint(std::istream& in, std::ostream& out, ManyDatasets_t& ds);
  void cmdComplement(std::istream& in, std::ostream& out, ManyDatasets_t& ds);
  void cmdIntersect(std::istream& in, std::ostream& out, ManyDatasets_t& ds);
  void cmdUnion(std::istream& in, std::ostream& out, ManyDatasets_t& ds);
};

#endif
