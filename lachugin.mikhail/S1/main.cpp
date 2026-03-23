#include <iostream>
#include "tools.hpp"
#include "list.hpp"

int main()
{
  try
  {
    lachugin::List< lachugin::pair > l = lachugin::getline(std::cin);

    lachugin::output(lachugin::process(l), l);
  }
  catch (...)
  {
    return 1;
  }
}
