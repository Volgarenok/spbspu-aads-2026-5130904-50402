#include "actions.hpp"
#include "list.hpp"
#include <iostream>
#include <string>

void dirko::input(std::istream &is, l_pair_t &lst)
{
  std::string title;
  while (is >> title) {
    List< long long > nums;
    long long num;
    while (is >> num) {
      nums.push_back(num);
    }
    is.clear();
    lst.push_back({title, nums});
  }
}
