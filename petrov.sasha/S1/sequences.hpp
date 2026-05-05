#ifndef SEQUENCES_HPP
#define SEQUENCES_HPP

#include <string>
#include <utility>
#include <iostream>
#include <cstddef>
#include <limits>
#include "list.hpp"

namespace petrov {

  const std::size_t max_size_value = std::numeric_limits< std::size_t >::max();

  using NumberList = List< std::size_t >;
  using NamedSequence = std::pair< std::string, NumberList >;
  using SequenceList = List< NamedSequence >;
  using RowList = List< NumberList >;

  bool readSequences(std::istream& input, SequenceList& sequences);
  std::size_t getMaxLength(const SequenceList& sequences);
  void buildRows(const SequenceList& sequences, RowList& rows);
  bool buildSums(const RowList& rows, NumberList& sums);

  void printNames(std::ostream& output, const SequenceList& sequences);
  void printRows(std::ostream& output, const RowList& rows);
  void printSums(std::ostream& output, const NumberList& sums);

}

#endif
