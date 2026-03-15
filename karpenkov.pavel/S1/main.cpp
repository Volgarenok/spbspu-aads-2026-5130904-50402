#include <iostream>
#include "listProcessing.hpp"
#include "list.hpp"

int main()
{
  karpenkov::pair_list sequences;
  try {
    karpenkov::readSequences(std::cin, sequences);
    if (sequences.cbegin() == sequences.cend()) {
      std::cout << 0 << '\n';
      return 0;
    }
    karpenkov::printNames(sequences, std::cout);
    karpenkov::printSequences(sequences, std::cout);
  } catch (...) {
    std::cerr << "Memory allocation error" << '\n';
    return 1;
  }
  return 0;
}
