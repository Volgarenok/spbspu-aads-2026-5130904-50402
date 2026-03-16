#include <iostream>
#include <stdexcept>
#include "actions.hpp"
#include "list.hpp"

int main()
{
  dirko::l_pair_t seq;
  dirko::input(std::cin, seq);
  if (seq.size() == 0) {
    std::cout << "0\n";
    return 0;
  }
  dirko::List< dirko::List< size_t > > list = dirko::process(seq);
  printNames(std::cout, seq) << '\n';
  printList(std::cout, list);
  try {
    dirko::printSum(std::cout, list) << '\n';
  } catch (const std::overflow_error &e) {
    std::cerr << "overflow\n";
    return 1;
  }
}
