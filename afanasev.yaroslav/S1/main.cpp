#include "list_in_out.hpp"
#include <iostream>

int main()
{
  namespace a = afanasev;

  try
  {
    a::List< std::pair< std::string, a::List< size_t > > > list;
    bool flag = a::input(std::cin, list);

    if (flag)
    {
      a::output(std::cout, list);
    }
    else
    {
      std::cout << "0\n";
    }
  }
  catch (const std::overflow_error & e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
  return 0;
}
