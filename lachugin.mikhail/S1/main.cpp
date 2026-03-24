#include <iostream>
#include "tools.hpp"
#include "list.hpp"

int main()
{
  try
  {
    auto l = lachugin::getline(std::cin);
    auto res = lachugin::process(l);
    lachugin::output(res, l);

    auto sums = lachugin::listSums(res);
    auto it = sums.begin();
    bool isFirst = true;

    while (it != sums.end())
    {
      if (isFirst)
      {
        std::cout << *it;
        isFirst = false;
      }
      else
      {
        std::cout << " " << *it;
      }
      ++it;
    }
    std::cout << "\n";
  }
  catch (const std::overflow_error& e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
}
