#include "function.hpp"
#include <iostream>
#include <string>
#include <ostream>
#include <algorithm>

namespace muh = muhamadiarov;

void muh::input(std::istream& in, l_pair_str_l_t& list)
{
  std::string name;
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

muh::List< muh::List< size_t > > muh::transposes(const l_pair_str_l_t& list)
{
  size_t maxLen = 0;
  c_iter_t lciter = list.cbegin(); 
  while (lciter != list.cend())
  {
    maxLen = std::max(maxLen, (*lciter).second.size());
    ++lciter;
  }
  List< List < size_t > > result;
  for (size_t i = 0; i < maxLen; ++i)
  {
    List< size_t > listNum;
    lciter = list.cbegin();
    for (size_t j = 0; j < list.size(); ++j)
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
      ++lciter;
    }
    result.pushFront(listNum);
  }
  return result;
}

void muh::printList(std::ostream& out, const List< List< size_t > >& list)
{
  LCIter< List< size_t > > iter = list.cbegin();
  for (size_t i = 0; i < list.size(); ++i)
  {
    const List< size_t >& inList = *iter;
    if (inList.size() != 0)
    {
      LCIter< size_t > podIter = inList.cbegin();
      out << *podIter;
      for (size_t j = 1; j < inList.size(); ++j)
      {
        ++poditer;
        out << ' ' << *poditer;
      }
    }
    out << '\n';
    ++iter;
  }
}

void muh::printNames(std::ostream& out, const l_pair_str_l_t& list)
{
  c_iter_t iter = list.cbegin();
  while (iter != list.cend())
  {
    out << (*iter).first;
    ++iter;
    if (iter != list.cend())
    {
      out << " ";
    }
  }
}
