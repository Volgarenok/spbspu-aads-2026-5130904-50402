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
      List< int > list;
      Node< int >* curr = nullptr;

      while (true)
      {
        int c = in.peek();
        if (c == '\n' || c == EOF)
        {
          break;
        }
        if (!std::isdigit(c) && c != '-')
        {
          break;
        }
        long long tmp;
        in >> tmp;
        if (tmp > std::numeric_limits< int >::max() ||
            tmp < std::numeric_limits< int >::min())
        {
          std::cerr << "overflow\n";
          throw std::overflow_error("overflow");
        }
        int value = static_cast< int >(tmp);
        if (curr == nullptr)
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
  catch (...)
  {
    listOfLists.clear();
    std::cerr << "Error in getline" << "\n";
    throw;
  }
  return listOfLists;
}

lachugin::List< lachugin::List< int > > lachugin::process(List< pair > l)
{
  List< List< int > > listOfLists{};
  Node< List< int > >* currOfLists = nullptr;
  size_t k = 0;
  List< List< int > > zeroResult{};
  try
  {
    if (l.begin() == l.end())
    {
      List< int > zeroList;
      zeroList.add(0);
      zeroResult.add(zeroList);
    }

    while (true)
    {
      List< int > listOfVal{};
      Node< int >* curr = nullptr;
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

    List< int > sumList{};
    Node< int >* currSum = nullptr;
    auto itLists = listOfLists.begin();

    while (itLists != listOfLists.end())
    {
      int sum = 0;
      auto itVals = (*itLists).begin();
      while (itVals != (*itLists).end())
      {
        sum += *itVals;
        ++itVals;
      }

      if (currSum == nullptr)
      {
        currSum = sumList.add(sum);
      }
      else
      {
        currSum = sumList.addNext(sum, currSum);
      }
      ++itLists;
    }

    if (currOfLists == nullptr)
    {
      listOfLists.add(sumList);
    }
    else
    {
      listOfLists.addNext(sumList, currOfLists);
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

void lachugin::output(const List< List< int > >& lVal, const List< pair >& l)
{
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
      bool isFirst = true;

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

