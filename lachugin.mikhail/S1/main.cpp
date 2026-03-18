#include <iostream>
#include "tools.hpp"
#include "list.hpp"

int main()
{
  lachugin::List< lachugin::pair > l;
  try
  {
    std::cout << "beg\n";
    l = lachugin::getline(std::cin);
    std::cout << "end\n";
    if (l.begin() == l.end())
    {
      std::cout << "EMPTY\n";
    }
    else
    {
      std::cout << "NOT EMPTY\n";
    }

    lachugin::output(lachugin::process(l), l);
  }
  catch (...)
  {
    return 1;
  }
}
