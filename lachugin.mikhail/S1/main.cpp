#include <iostream>
#include "tools.hpp"
#include "../common/list.hpp"

int main()
{
  try
  {
    auto l = lachugin::getline(std::cin);
    if (l.begin() == l.end())
    {
      std::cout << "0\n";
      return 0;
    }

    auto res = lachugin::process(l);
    lachugin::output(res, l);
    auto sums = lachugin::listSums(res);
    auto it = sums.begin();
    bool first = true;

    while (it != sums.end())
    {
      if (first)
      {
        std::cout << *it;
        first = false;
      }
      else
      {
        std::cout << " " << *it;
      }
      ++it;
    }
    std::cout << "\n";
  }
  catch (const std::overflow_error&)
  {
    std::cerr << "overflow\n";
    return 1;
  }
}
