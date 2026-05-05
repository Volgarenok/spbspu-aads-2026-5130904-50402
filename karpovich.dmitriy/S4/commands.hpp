#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <hashFunctions.hpp>
#include <hashTable.hpp>
#include <iostream>
#include <string>
#include "bstree.hpp"

namespace karpovich
{
  using Dataset = BSTree< int, std::string >;
  using Datasets = HashTable< std::string, Dataset, Hasher< std::string >, std::equal_to< std::string > >;

  void cmdPrint(std::istream &in, std::ostream &out, Datasets &datasets);
  void cmdComplement(std::istream &in, std::ostream &out, Datasets &datasets);
  void cmdIntersect(std::istream &in, std::ostream &out, Datasets &datasets);
  void cmdUnion(std::istream &in, std::ostream &out, Datasets &datasets);
}

#endif
