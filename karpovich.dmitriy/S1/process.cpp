#include "process.hpp"
#include <cstddef>

void karpovich::input(std::istream &in, list_pair_t &list)
{
  std::string name;
  while (in >> name) {
    List< size_t > numbers;
    size_t num = 0;
    while (in.peek() != '\n' && !in.eof()) {
      if (in >> num) {
        numbers.push_back(num);
      } else {
        break;
      }
    }
    if (in.peek() == '\n') {
      in.get();
    }
    list.push_back(std::make_pair(name, numbers));
  }
}

void karpovich::transpose(const list_pair_t &list, List< List< size_t > > &data)
{
  size_t max_len = 0;
  for (auto it = list.begin(); it != list.end(); it++) {
    if ((*it).second.size() > max_len) {
      max_len = (*it).second.size();
    }
  }
  for (size_t i = 0; i < max_len; i++) {
    List< size_t > row_numbers;
    for (auto seq_it = list.begin(); seq_it != list.end(); seq_it++) {
      if (i < (*seq_it).second.size()) {
        auto num_it = (*seq_it).second.begin();
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
  for (auto it = names.begin(); it != names.end(); ++it) {
    if (!first) {
      out << ' ';
    }
    out << (*it).first;
    first = false;
  }
  out << '\n';

  for (auto row_it = transposed.begin(); row_it != transposed.end(); ++row_it) {
    first = true;
    for (auto num_it = (*row_it).begin(); num_it != (*row_it).end(); ++num_it) {
      if (!first) {
        out << ' ';
      }
      out << *num_it;
      first = false;
    }
    out << '\n';
  }

  first = true;
  for (auto row_it = transposed.begin(); row_it != transposed.end(); ++row_it) {
    size_t sum = 0;
    for (auto num_it = (*row_it).begin(); num_it != (*row_it).end(); ++num_it) {
      sum += *num_it;
    }
    if (!first) {
      out << ' ';
    }
    out << sum;
    first = false;
  }
  out << '\n';
}
