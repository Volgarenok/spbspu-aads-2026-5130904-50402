#include <iostream>
#include <stdexcept>
#include "actions.hpp"
#include "iters.hpp"
#include "list.hpp"

int main()
{
  dirko::l_pair_t seq;
  try {
    dirko::input(std::cin, seq);
  } catch (const std::overflow_error &e) {
    std::cerr << "overflow\n";
    return 1;
  }
  if (seq.size() == 0) {
    std::cout << "0\n";
    return 0;
  }
  dirko::List< dirko::List< long long > > list = dirko::process(seq);
  try {
    dirko::output(std::cout, seq, list);
  } catch (const std::overflow_error &e) {
    std::cerr << "overflow\n";
    return 1;
  }
}
