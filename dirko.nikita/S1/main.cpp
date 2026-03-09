#include <iostream>
#include <string>
#include <utility>
#include "actions.hpp"
#include "iters.hpp"
#include "list.hpp"

int main()
{
  dirko::List< std::pair< std::string, dirko::List< long long > > > seq;
  dirko::input(std::cin, seq);
}
