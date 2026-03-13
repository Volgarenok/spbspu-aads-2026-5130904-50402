#include "list.hpp"

#include <iostream>
#include <string>
#include <utility>

int main()
{}

bool petrov::readSequences(std::istream& input, SequenceList& sequences) {
  while (true) {
    std::string name = "";
    if (!(input >> name)) {
      if (input.eof()) {
        return true;
      }
      return false;
    }
    NumberList numbers;
    while(true) {
      const int next_symbol = input.peek();
      if (next_symbol == '\n') {
        input.get();
        break;
      }
      if (next_symbol == std::char_traits< char >::eof()) {
        break;
      }
      size_t number = 0;
      if (!(input >> number)) {
        return false;
      }
      numbers.pushBack(number);
    }
    sequences.pushBack(std::make_pair(name, std::move(numbers)));

    if (input.eof()) {
      break;
    }
  }
  return true;
}

size_t petrov::getMaxLength(const SequenceList& sequences) {
  size_t max_lenght = 0;
  for (SequenceList::const_iterator it = sequences.cbegin(); it != sequences.cend(); ++it) {
    const size_t current_size = it->second.size();
    max_lenght = (max_lenght < current_size) ? current_size : max_lenght;
  }
  return max_lenght;
}

void petrov::buildRows(const SequenceList& sequences, RowList& rows) {
  const size_t max_lenght = getMaxLength(sequences);
  for (size_t position = 0; position < max_lenght; ++position) {
    NumberList row;
    for (SequenceList::const_iterator seq_it = sequences.cbegin(); seq_it != sequences.cend(); ++seq_it) {
      NumberList::const_iterator num_it = seq_it->second.cbegin();
      size_t current_index = 0;

      while (current_index < position && num_it != seq_it->second.cend()) {
        ++num_it;
        ++current_index;
      }
      if (num_it != seq_it->second.cend()) {
        row.pushBack(*num_it);
      }
    }
    if (!(row.empty())) {
      rows.pushBack(std::move(row));
    }
  }
}

bool petrov::buildSums(const RowList& rows, NumberList& sums) {
  for (RowList::const_iterator row_it = rows.cbegin(); row_it != rows.cend(); ++row_it) {
    size_t sum = 0;
    for (NumberList::const_iterator num_it = row_it->cbegin(); num_it != row_it->cend(); ++num_it) {
      if (sum > max_size_value - *num_it) {
        return false;
      }
      sum += *num_it;
    }
    sums.pushBack(sum);
  }
}
