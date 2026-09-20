#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <cstddef>
#include <string>
#include <utility>
#include "list.hpp"

namespace pozdnyakov
{

  using ValueType = size_t;
  using NamedSequence = std::pair< std::string, List< ValueType > >;

  List< List< ValueType > > buildInterleavedRows(List< NamedSequence > &sequences);
  List< ValueType > calculateSums(List< List< ValueType > > &rows);

}

#endif
