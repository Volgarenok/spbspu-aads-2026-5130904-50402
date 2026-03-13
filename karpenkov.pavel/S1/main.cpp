#include <iostream>
#include "listProcessing.hpp"
#include "list.hpp"

int main()
{
  karpenkov::pair_list sequences;
  karpenkov::readSequences(std::cin, sequences);
  if (sequences.begin() == sequences.end()) {
    std::cout << 0 << std::endl;
    return 0;
  }
  karpenkov::printNames(sequences, std::cout);
  karpenkov::printSequences(sequences, std::cout);

  return 0;
}