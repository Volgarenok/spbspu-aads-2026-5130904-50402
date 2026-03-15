#include "iters.hpp"
#include "list.hpp"
#include "node.hpp"
#include "funcs.hpp"
#include <iostream>

int main()
{
  auto list = strelnikov::input(std::cin);
  if (list.cbegin() == list.cend()) {
    std::cerr << "\n0\n";
    return 0;
  }

  auto onlynums = leave_nums(list);

  strelnikov::List< size_t > sums;
  try {
    sums = get_sum(onlynums);
  } catch (const std::overflow_error &e) {
    strelnikov::printNames(std::cout, list) << '\n';
    strelnikov::printList(std::cout, onlynums) << '\n';
    std::cerr << e.what();
    return 1;
  }

  strelnikov::printNames(std::cout, list) << '\n';
  if(onlynums.cbegin() == nullptr) {
    std::cout << "0\n";
    return 0;
  }
  strelnikov::printList(std::cout, onlynums) << '\n';
  strelnikov::printSum(std::cout, sums) << '\n';
}
