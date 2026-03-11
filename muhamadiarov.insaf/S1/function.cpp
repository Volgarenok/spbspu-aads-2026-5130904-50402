#include "function.hpp"
#include <iostream>
#include <string>
#include <ostream>
#include <algorithm>

namespace muh = muhamadiarov;

void muh::input(std::istream& in, l_pair_str_l_t& list)
{
  std::string name = "";
  while (in >> name)
  {
    List< size_t > listNum;  
    size_t num = 0;
    while (in >> num)
    {
      listNum.pushFront(num);
    }
    list.pushFront({name, listNum});
    in.clear();
  }
}

muh::List< muh::list< size_t > > muh::transposes(const l_pair_str_l_t& list)
{
  size_t maxLen = 0;
  c_iter_t lciter = list.cbegin(); 
  while (lciter != list.cend())
  {
    maxLen = std::max(maxLen, (*lciter).second.size());
    ++lciter;
  }
  List< List < size_t > >* result = nullptr;
  for (size_t i = 0; i < maxLen; ++i)
  {
    List< size_t > listNum = nullptr;
    lciter = list.cbegin();
    size_t j = 0;
    while (j != list.size())
    {
      if ((*lciter).second.size() > i)
      {
	LCIter< size_t > iter = ((*lciter).second).cbegin();
	for (size_t c = 0; c < i; ++c)
	{
	  ++iter;
	}
        listNum.pushFront(*iter);
      }
      ++j;
    }
    result.pushFront(listNum);
    ++lciter;
  }
  return result;
}
