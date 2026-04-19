#include "process.hpp"
#include <cstddef>
#include <iterators.hpp>
#include <limits>
#include <list.hpp>
#include <stdexcept>
#include <utility>

void karpovich::input(std::istream &in, list_pair_t &list)
{
  std::string name;
  while (in >> name) {
    List< size_t > numbers;
    size_t num;
    while (in >> num) {
      numbers.push_back(num);
    }
    in.clear();
    list.push_back({name, numbers});
  }
}

void karpovich::transpose(const list_pair_t &list, List< List< size_t > > &data)
{
  size_t max_len = 0;
  for (LCIter< std::pair< std::string, List< size_t > > > it = list.cbegin(); it != list.cend(); ++it) {
    if ((*it).second.size() > max_len) {
      max_len = (*it).second.size();
    }
  }
  for (size_t i = 0; i < max_len; i++) {
    List< size_t > row_numbers;
    for (LCIter< std::pair< std::string, List< size_t > > > seq_it = list.cbegin(); seq_it != list.cend(); ++seq_it) {
      if (i < (*seq_it).second.size()) {
        LCIter< size_t > num_it = (*seq_it).second.cbegin();
        for (size_t j = 0; j < i; ++j) {
          ++num_it;
        }
        row_numbers.push_back(*num_it);
      }
    }
    if (row_numbers.size() > 0) {
      data.push_back(row_numbers);
    }
  }
}

void karpovich::printNames(std::ostream &out, const list_pair_t &names)
{
  bool first = true;
  for (LCIter< std::pair< std::string, List< size_t > > > it = names.cbegin(); it != names.cend(); ++it) {
    if (!first) {
      out << ' ';
    }
    out << (*it).first;
    first = false;
  }
  out << '\n';
}

void karpovich::printTransposed(std::ostream &out, const List< List< size_t > > &transposed)
{
  bool first = true;
  for (LCIter< List< size_t > > row_it = transposed.cbegin(); row_it != transposed.cend(); ++row_it) {
    first = true;
    for (LCIter< size_t > num_it = (*row_it).cbegin(); num_it != (*row_it).cend(); ++num_it) {
      if (!first) {
        out << ' ';
      }
      out << *num_it;
      first = false;
    }
    out << '\n';
  }
}

void karpovich::output(std::ostream &out, const list_pair_t &names, const List< List< size_t > > &transposed)
{
  printNames(out, names);
  printTransposed(out, transposed);
  bool first = true;
  if (transposed.size() == 0) {
    out << "0\n";
  } else {
    List< size_t > sums;
    for (LCIter< List< size_t > > row_it = transposed.cbegin(); row_it != transposed.cend(); ++row_it) {
      size_t sum = 0;
      for (LCIter< size_t > num_it = (*row_it).cbegin(); num_it != (*row_it).cend(); ++num_it) {
        if (*num_it > std::numeric_limits< size_t >::max() - sum) {
          throw std::overflow_error("overflow");
          return;
        }
        sum += *num_it;
      }
      sums.push_back(sum);
    }
    for (LIter< size_t > it = sums.begin(); it != sums.end(); ++it) {
      if (!first) {
        out << ' ';
      }
      out << *it;
      first = false;
    }
    out << '\n';
  }
}
