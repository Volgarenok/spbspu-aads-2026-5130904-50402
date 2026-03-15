#include "tools.hpp"
#include "list.hpp"
#include "itters.hpp"
#include <iostream>

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
      int value;

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

lachugin::List< lachugin::List< int > > lachugin::process(List< pair > l) {
  List< List< int > > listOfLists{};
  Node< List< int > >* currOfLists = nullptr;

  size_t k = 0;

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

  List<int> sumList{};
  Node<int>* currSum = nullptr;

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
  return listOfLists;
}
