#include "actions.hpp"
#include <iostream>
#include <limits>
#include <ostream>
#include <stdexcept>
#include <string>

void dirko::input(std::istream &is, l_pair_t &seq)
{
  std::string title;
  while (is >> title) {
    List< size_t > nums;
    size_t num;
    while (is >> num) {
      nums.push_back(num);
    }
    is.clear();
    seq.push_back({title, nums});
  }
}

dirko::List< dirko::List< size_t > > dirko::process(const l_pair_t &seq)
{
  size_t maxLen = 0;
  ci_pair_t seq_iter = seq.cbegin();
  while (seq_iter != seq.cend()) {
    maxLen = std::max(maxLen, (*seq_iter).second.size());
    ++seq_iter;
  }
  List< List< size_t > > list;
  for (size_t i = 0; i < maxLen; ++i) {
    List< size_t > row;
    seq_iter = seq.cbegin();
    while (seq_iter != seq.cend()) {
      if (i < (*seq_iter).second.size()) {
        CIter< size_t > row_iter = (*seq_iter).second.cbegin();
        for (size_t j = 0; j < i; ++j) {
          ++row_iter;
        }
        row.push_back(*row_iter);
      }
      ++seq_iter;
    }
    list.push_back(row);
  }
  return list;
}

std::ostream &dirko::printSum(std::ostream &os, const List< List< size_t > > &list)
{
  if (list.size() == 0) {
    return os << "0";
  }
  os << '\n';
  CIter< List< size_t > > iter = list.cbegin();
  List< size_t > sums;
  while (iter != list.cend()) {
    size_t sum = 0;
    CIter< size_t > it = (*iter).cbegin();
    while (it != (*iter).cend()) {
      if (std::numeric_limits< size_t >::max() - sum < *it) {
        throw std::overflow_error("overflow_error");
      }
      sum += *it;
      ++it;
    }
    sums.push_back(sum);
    ++iter;
  }
  CIter< size_t > it = sums.cbegin();
  os << *it;
  ++it;
  while (it != sums.cend()) {
    os << ' ' << *it;
    ++it;
  }
  return os;
}

std::ostream &dirko::printNames(std::ostream &os, const l_pair_t &names)
{
  ci_pair_t iter = names.cbegin();
  os << (*iter).first;
  ++iter;
  while (iter != names.cend()) {
    os << ' ' << (*iter).first;
    ++iter;
  }
  return os;
}

std::ostream &dirko::printList(std::ostream &os, const List< List< size_t > > &list)
{
  bool lf = false;
  CIter< List< size_t > > iter = list.cbegin();
  while (iter != list.cend()) {
    if (lf) {
      os << '\n';
    }
    CIter< size_t > it = (*iter).cbegin();
    os << *it;
    ++it;
    while (it != (*iter).cend()) {
      os << ' ' << *it;
      ++it;
    }
    ++iter;
    lf = true;
  }
  return os;
}
