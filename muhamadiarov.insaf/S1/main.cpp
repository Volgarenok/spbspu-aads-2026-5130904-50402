#include <iostream>
#include <algorithm>
#include <stdexcept>
#include "function.hpp"
#include "liter.hpp"
#include "lciter.hpp"
#include "list.hpp"

int main()
{
  namespace muh = muhamadiarov;
  muh::l_pair_str_l_t list;
  muh::input(std::cin, list);
  if (list.size() == 0)
  {
    std::cout << "0\n";
    return 0;
  }
  muh::List< muh::List< size_t > > trList = muh::transposes(list);
  muh::printNames(std::cout, list);
  std::cout << '\n';
  muh::printList(std::cout, trList);
  std::cout << '\n';
  try
  {
    muh::printSum(std::cout, trList);
    std::cout << '\n';
  }
  catch (const std::overflow_error& e)
  {
    std::cerr << "overflow\n";
    return 1;
  }
  return 0;
}
