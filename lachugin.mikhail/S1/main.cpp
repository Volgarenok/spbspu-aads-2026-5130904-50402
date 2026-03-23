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
  catch (const std::overflow_error& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
  catch (...)
  {
    return 1;
  }
}
