#include <iostream>
#include "tools.hpp"
#include "list.hpp"

int main()
{
  lachugin::List< lachugin::pair > l;
  try
  {
    l = lachugin::getline(std::cin);

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
  catch (const std::bad_alloc& e)
  {
    return 1;
  }
}
