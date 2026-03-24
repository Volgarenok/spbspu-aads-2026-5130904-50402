#include <iostream>
#include "sequences.hpp"

int main()
{
  petrov::SequenceList sequences;
  if (!petrov::readSequences(std::cin, sequences)) {
    std::cerr << "Input error\n";
    return 1;
  }
  if (sequences.empty()) {
    std::cout << "0\n";
    return 0;
  }

  petrov::printNames(std::cout, sequences);
  petrov::RowList rows;
  petrov::buildRows(sequences, rows);

  if (rows.empty()) {
    std::cout << "0\n";
    return 0;
  }

  petrov::printRows(std::cout, rows);
  petrov::NumberList sums;
  if (!petrov::buildSums(rows, sums)) {
    std::cerr << "Sum overflow\n";
    return 1;
  }
  petrov::printSums(std::cout, sums);
  return 0;
}
