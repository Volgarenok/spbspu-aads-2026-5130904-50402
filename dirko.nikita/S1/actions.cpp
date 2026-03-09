#include "actions.hpp"
#include "iters.hpp"
#include "list.hpp"
#include <iostream>
#include <string>

void dirko::input(std::istream &is, l_pair_t &seq)
{
  std::string title;
  while (is >> title) {
    List< long long > nums;
    long long num;
    while (is >> num) {
      nums.push_back(num);
    }
    is.clear();
    seq.push_back({title, nums});
  }
}

dirko::List< dirko::List< long long > > dirko::process(l_pair_t seq)
{
  size_t maxLen = 0;
  ci_pair_t seq_iter = seq.cbegin();
  while (seq_iter != seq.cend()) {
    maxLen = std::max(maxLen, (*seq_iter).second.size());
    ++seq_iter;
  }
  List< List< long long > > list;
  for (size_t i = 0; i < maxLen; ++i) {
    List< long long > row;
    seq_iter = seq.cbegin();
    while (seq_iter != seq.cend()) {
      if (i < (*seq_iter).second.size()) {
        CIter< long long > row_iter = (*seq_iter).second.cbegin();
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

void dirko::output(std::ostream &os, l_pair_t titles, List< List< long long > > list)
{
  printNames(os, titles);
  os << '\n';
  printList(os, list);
  os << '\n';
  CIter< List< long long > > iter = list.cbegin();
  bool space = false;
  while (iter != list.cend()) {
    long long sum = 0;
    CIter< long long > it = (*iter).cbegin();
    while (it != (*iter).cend()) {
      sum += *it;
      ++it;
    }
    if (space) {
      os << ' ';
    }
    os << sum;
    ++iter;
    space = true;
  }
}

void dirko::printNames(std::ostream &os, l_pair_t names)
{
  ci_pair_t iter = names.cbegin();
  os << (*iter).first;
  ++iter;
  while (iter != names.cend()) {
    os << ' ' << (*iter).first;
    ++iter;
  }
}

void dirko::printList(std::ostream &os, List< List< long long > > list)
{
  bool lf = false;
  CIter< List< long long > > iter = list.cbegin();
  while (iter != list.cend()) {
    if (lf) {
      os << '\n';
    }
    CIter< long long > it = (*iter).cbegin();
    os << *it;
    ++it;
    while (it != (*iter).cend()) {
      os << ' ' << *it;
      ++it;
    }
    ++iter;
    lf = true;
  }
}
