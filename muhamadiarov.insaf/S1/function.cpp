#include "function.hpp"
#include <iostream>

namespace muh = muhamadiarov;

muh::void input(std::istream& in, l_pair_str_l_t& list)
{
  std::string name = "";
  while (in >> name)
  {
    List< size_t > listNum = {0, nullptr, nullptr};  
    size_t num = 0;
    while (in >> num)
    {
      listNum.pushFront(num);
    }
    list.pushFront({name, listNum});
    in.clear();
  }
} 
