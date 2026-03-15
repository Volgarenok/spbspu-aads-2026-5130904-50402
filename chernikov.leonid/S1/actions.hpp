#ifndef ACTIONS_HPP
#define ACTIONS_HPP

#include "List.hpp"
#include <string>
#include <utility>

namespace chernikov {

  using Sequence = std::pair< std::string, List< int > >;

  List< Sequence > parser(std::istream &in);
  void print_names(std::ostream &out, const List< Sequence > &sequences);
  size_t max_sequence_length(const List< Sequence > &sequences);
  void print_transposed(std::ostream &out, const List< Sequence > &sequences);
  void print_sums(std::ostream &out, const List< Sequence > &sequences);

}

#endif