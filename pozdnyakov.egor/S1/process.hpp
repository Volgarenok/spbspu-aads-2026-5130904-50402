#ifndef POZDNYAKOV_PROCESS_HPP
#define POZDNYAKOV_PROCESS_HPP

#include "list.hpp"
#include <string>
#include <utility>

namespace pozdnyakov
{

  using ValueType = unsigned long long;
  using NamedSequence = std::pair< std::string, List< ValueType > >;

  struct ProcessResult
  {
    List< List< ValueType > > rows;
    List< ValueType > sums;
  };

  ProcessResult buildInterleavedRows(const List< NamedSequence > &sequences);

}

#endif
