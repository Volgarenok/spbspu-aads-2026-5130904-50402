#include <iostream>
#include "task.hpp"

int main()
{
  namespace per = permyakov;
  using list_t = per::List < std::pair < std::string, per::List < size_t > > >;
  list_t bgList;
  per::input(std::cin, bgList);
  //std::cout << bgList.front().second.front();
  per::outputName (std::cout, bgList);
  per::outputNum (std::cout, bgList.front().second);
}