#include "process.hpp"
#include "iterators.hpp"
#include "list.hpp"
#include <cstddef>
#include <limits>
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
  for (LCIter< std::pair< std::string, List< size_t > > > it = list.begin(); it != list.end(); it++) {
    if ((*it).second.size() > max_len) {
      max_len = (*it).second.size();
    }
  }
  for (size_t i = 0; i < max_len; i++) {
    List< size_t > row_numbers;
    for (LCIter< std::pair< std::string, List< size_t > > > seq_it = list.begin(); seq_it != list.end(); seq_it++) {
      if (i < (*seq_it).second.size()) {
        LCIter< size_t > num_it = (*seq_it).second.begin();
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

void karpovich::output(std::ostream &out, const list_pair_t &names, const List< List< size_t > > &transposed)
{
  bool first = true;
  for (LCIter< std::pair< std::string, List< size_t > > > it = names.begin(); it != names.end(); ++it) {
    if (!first) {
      out << ' ';
    }
    out << (*it).first;
    first = false;
  }
  out << '\n';

  for (LCIter< List< size_t > > row_it = transposed.begin(); row_it != transposed.end(); ++row_it) {
    first = true;
    for (LCIter< size_t > num_it = (*row_it).begin(); num_it != (*row_it).end(); ++num_it) {
      if (!first) {
        out << ' ';
      }
      out << *num_it;
      first = false;
    }
    out << '\n';
  }

  first = true;
  if (transposed.size() == 0) {
    out << "0\n";
  } else {
    List< size_t > sums;
    for (LCIter< List< size_t > > row_it = transposed.begin(); row_it != transposed.end(); ++row_it) {
      size_t sum = 0;
      for (LCIter< size_t > num_it = (*row_it).begin(); num_it != (*row_it).end(); ++num_it) {
        if (*num_it > std::numeric_limits< size_t >::max() - sum) {
          throw std::overflow_error("overflow");
          return;
        }
        sum += *num_it;
      }
      sums.push_back(sum);
    }
    for (LIter< size_t > it = sums.begin(); it != sums.end(); it++) {
      if (!first) {
        out << ' ';
      }
      out << *it;
      first = false;
    }
    out << '\n';
  }
}
