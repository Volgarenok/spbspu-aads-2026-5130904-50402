#include "tools.hpp"
#include "list.hpp"
#include "itters.hpp"
#include <iostream>
#include <limits>

lachugin::List < lachugin::pair > lachugin::getline(std::istream& in)
{
  List < pair > listOfLists{};
  Node < pair >* currOfLists = nullptr;
  std::string name;
  try
  {
    while (in >> name)
    {
      List< size_t > list;
      Node< size_t >* curr = nullptr;
      size_t value;

      while(in >> value)
      {
        if(curr == nullptr)
        {
          curr = list.add(value);
        }
        else
        {
          curr = list.addNext(value, curr);
        }
      }

      in.clear();
      pair p{name, list};

      if (currOfLists == nullptr)
      {
        currOfLists = listOfLists.add(p);
      }
      else
      {
        currOfLists = listOfLists.addNext(p, currOfLists);
      }
    }
  }
  catch (const std::bad_alloc& e)
  {
    std::cout << e.what();
    listOfLists.clear();
    throw;
  }
  return listOfLists;
}

lachugin::List< lachugin::List< size_t > > lachugin::process(List< pair > l)
{
  List< List< size_t > > listOfLists{};
  Node< List< size_t > >* currOfLists = nullptr;
  size_t k = 0;
  List< List< size_t > > zeroResult{};
  try
  {
    if (l.begin() == l.end())
    {
      List< size_t > zeroList;
      zeroList.add(0);
      zeroResult.add(zeroList);
    }

    while (true)
    {
      List< size_t > listOfVal{};
      Node< size_t >* curr = nullptr;
      bool added = false;
      auto iterator = l.begin();

      while (iterator != l.end())
      {
        auto it = (*iterator).second.begin();
        size_t i = 0;
        while (i < k && it != (*iterator).second.end())
        {
          ++it;
          ++i;
        }

        if (it != (*iterator).second.end())
        {
          if (curr == nullptr)
          {
            curr = listOfVal.add(*it);
          }
          else
          {
            curr = listOfVal.addNext(*it, curr);
          }
          added = true;
        }

        ++iterator;
      }

      if (!added)
      {
        break;
      }
      if (currOfLists == nullptr)
      {
        currOfLists = listOfLists.add(listOfVal);
      }
      else
      {
        currOfLists = listOfLists.addNext(listOfVal, currOfLists);
      }
      ++k;
    }
  }
  catch (const std::bad_alloc& e)
  {
    std::cout << e.what();
    auto itDelete = listOfLists.begin();
    while (itDelete != listOfLists.end())
    {
      (*itDelete).clear();
      ++itDelete;
    }
    listOfLists.clear();
    throw;
  }
  if (l.begin() == l.end())
  {
    return zeroResult;
  } else
  {
    return listOfLists;
  }
}

lachugin::List< size_t > lachugin::listSums(List< List< size_t > > lVal)
{
  List< size_t > sumList{};
  Node< size_t >* curr = nullptr;
  auto itLists = lVal.begin();
  while (itLists != lVal.end())
  {
    size_t sum = 0;
    auto itVals = (*itLists).begin();
    while (itVals != (*itLists).end())
    {
      size_t val = *itVals;
      if (sum > std::numeric_limits< size_t >::min() - val)
      {
        throw std::overflow_error("overflow");
      }
      sum += val;
      ++itVals;
    }

    if (curr == nullptr)
    {
      curr = sumList.add(sum);
    }
    else
    {
      curr = sumList.addNext(sum, curr);
    }
    ++itLists;
  }
  return sumList;
}

void lachugin::output(List< List< size_t > >& lVal, List< pair >& l)
{
  if (l.begin() == l.end())
  {
    std::cout << "0\n";
    return;
  }
  auto it = l.begin();

  bool isFirst = true;
  while (it != l.end())
  {
    if (isFirst)
    {
      std::cout << (*it).first;
      isFirst = false;
    }
    else
    {
      std::cout << " " << (*it).first;
    }
    ++it;
  }
  std::cout << "\n";
  auto itVal = lVal.begin();

  while (itVal != lVal.end())
  {
    if ((*itVal).begin() == (*itVal).end())
    {
      std::cout << "0\n";
    }
    else
    {
      auto iterator = (*itVal).begin();
      isFirst = true;

      while (iterator != (*itVal).end())
      {
        if (isFirst)
        {
          std::cout << (*iterator);
          isFirst = false;
        }
        else
        {
          std::cout << ' ' << (*iterator);
        }
        ++iterator;
      }
      std::cout << "\n";
    }
    ++itVal;
  }
}

