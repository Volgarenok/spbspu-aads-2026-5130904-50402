#include <iostream>
#include <stdexcept>
#include "actions.hpp"
#include "iters.hpp"
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
  try {
    dirko::printSum(std::cout, seq, list) << '\n';
  } catch (const std::overflow_error &e) {
    std::cerr << "overflow\n";
    return 1;
  }
}
