#ifndef POZDNYAKOV_PROCESS_HPP
#define POZDNYAKOV_PROCESS_HPP

#include "list.hpp"
#include <cstddef>
#include <string>
#include <utility>

namespace pozdnyakov
{

  using ValueType = std::size_t;
  using NamedSequence = std::pair< std::string, List< ValueType > >;

  List< List< ValueType > > buildInterleavedRows(const List< NamedSequence > &sequences);
  List< ValueType > calculateSums(const List< List< ValueType > > &rows);

}

#endif
