#include "list.hpp"

#include <iostream>
#include <string>
#include <utility>

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
      std::size_t number = 0;
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

std::size_t petrov::getMaxLength(const SequenceList& sequences) {
  std::size_t max_length = 0;
  for (SequenceList::const_iterator it = sequences.cbegin(); it != sequences.cend(); ++it) {
    const std::size_t current_size = it->second.size();
    max_length = (max_length < current_size) ? current_size : max_length;
  }
  return max_length;
}

void petrov::buildRows(const SequenceList& sequences, RowList& rows) {
  const std::size_t max_length = getMaxLength(sequences);
  for (std::size_t position = 0; position < max_length; ++position) {
    NumberList row;
    for (SequenceList::const_iterator seq_it = sequences.cbegin(); seq_it != sequences.cend(); ++seq_it) {
      NumberList::const_iterator num_it = seq_it->second.cbegin();
      std::size_t current_index = 0;

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
    std::size_t sum = 0;
    for (NumberList::const_iterator num_it = row_it->cbegin(); num_it != row_it->cend(); ++num_it) {
      if (sum > max_size_value - *num_it) {
        return false;
      }
      sum += *num_it;
    }
    sums.pushBack(sum);
  }
  return true;
}

void petrov::printNames(std::ostream& output, const SequenceList& sequences) {
  SequenceList::const_iterator it = sequences.cbegin();
  if (it == sequences.cend()) {
    return;
  }
  output << it->first;
  ++it;
  while (it != sequences.cend()) {
    output << ' ' << it->first;
    ++it;
  }
  output << '\n';
}

void petrov::printRows(std::ostream& output, const RowList& rows) {
  for (RowList::const_iterator row_it = rows.cbegin(); row_it != rows.cend(); ++row_it) {
    NumberList::const_iterator num_it = row_it->cbegin();
    if (num_it != row_it->cend()) {
      output << *num_it;
      ++num_it;
    }
    while (num_it != row_it->cend()) {
      output << ' ' << *num_it;
      ++num_it;
    }
    output << '\n';
  }
}

void petrov::printSums(std::ostream& output, const NumberList& sums) {
  NumberList::const_iterator it = sums.cbegin();
  if (it == sums.cend()) {
    return;
  }
  output << *it;
  ++it;
  while (it != sums.cend()) {
    output << ' ' << *it;
    ++it;
  }
  output << '\n';
}

int main() {
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

