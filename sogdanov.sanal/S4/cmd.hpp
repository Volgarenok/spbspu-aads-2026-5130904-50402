#ifndef CMD_HPP
#define CMD_HPP

#include <iostream>
#include <string>

#include "map.hpp"

namespace sogdanov {

  using Dataset = Map< int, std::string >;
  using Datasets = Map< std::string, Dataset >;

  void cmdPrint(std::istream &in, std::ostream &out, Datasets &datasets);
  void cmdComplement(std::istream &in, std::ostream &out, Datasets &datasets);
  void cmdIntersect(std::istream &in, std::ostream &out, Datasets &datasets);
  void cmdUnion(std::istream &in, std::ostream &out, Datasets &datasets);

}

#endif
