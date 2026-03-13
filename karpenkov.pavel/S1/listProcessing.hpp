#ifndef LISTPROCESSING_HPP
#define LISTPROCESSING_HPP
#include "list.hpp"
namespace karpenkov{
  using pair_list = List<std::pair<std::string, List<size_t>>>;
  void readSequences(std::istream& input, pair_list& list);
  void printNames(const pair_list& list, std::ostream& output);
}
#endif
