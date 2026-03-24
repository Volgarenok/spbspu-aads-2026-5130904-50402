#include "ListImpl.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <cctype>

bool isNumber(std::string& str)
{
  for (char ch : str) {
    if (!std::isdigit(ch)) {
      return false;
    }
  }
  return true;
}

bool hasEls(ListTools::NamedLIter<int> lt, size_t depth)
{
  ListTools::NamedLIter<int> currIt = lt;
  while (currIt.curr)
  {
    if (currIt.value())
    {
      size_t d = 0;
      ListTools::LIter<int> temp(currIt.value());
      while (temp.curr)
      {
        if (depth == d)
        {
          return true;
        }
        d++;
        ++temp;
      }
    }
    ++currIt;
  }
  return false;
}

void printNames(ListTools::NamedLIter<int> lt)
{
  while (lt.hasNext())
  {
    std::cout << lt.getName() << " ";
    ++lt;
  }
  std::cout << lt.getName() << " ";
}

bool printEmbed(ListTools::LIter<int> em, size_t depth)
{
  for (size_t i = 0; i < depth; i++)
  {
    if (!em.hasNext())
    {
      return false;
    }
    ++em;
  }
  if (em.curr)
  {
    std::cout << em.value() << " ";
    return true;
  }
  else
  {
    return false;
  }
}

void printSeq(ListTools::NamedLIter<int> lt, ListTools::LIter<int> em, size_t depth)
{
  bool f = false;
  ListTools::NamedLIter<int> currIt = lt;
  if (hasEls(lt, depth+1))
  {
    while (currIt.curr)
    {
      if (currIt.value())
      {
        ListTools::LIter<int> temp(currIt.value());
        f = printEmbed(temp, depth);
      }
      ++currIt;
    }
    if (f)
    {
      std::cout << "\n";
      printSeq(lt, em, depth+1);
    }
  }
}

int main()
{
  ListTools::NamedList<int>* lhead = nullptr;
  ListTools::NamedLIter<int> lIt(lhead);
  ListTools::LIter<int> embedIt(nullptr);
  std::string data;
  while (std::cin >> data)
  {
    if (isNumber(data))
    {
      int number = std::stoi(data);
      if (!lIt.value())
      {
        ListTools::List<int>* embed = new ListTools::List<int>{number, nullptr};
        embedIt.set(embed);
        lIt.setData(embedIt.curr);
      }
      else
      {
        embedIt.insert(number);
        ++embedIt;
      }
    }
    else
    {
      if (!lhead)
      {
        lhead = new ListTools::NamedList<int>{data, nullptr, nullptr};
        lIt.setCurr(lhead);
      }
      else
      {
        lIt.insert(nullptr, data);
        ++lIt;
      }
    }
  }
  if (isNumber(data))
  {
    int number = std::stoi(data);
    if (!lIt.value())
    {
      ListTools::List<int>* embed = new ListTools::List<int>{number, nullptr};
      embedIt.set(embed);
      lIt.setData(embedIt.curr);
    }
    else
    {
      embedIt.insert(number);
      ++embedIt;
    }
  }
  else
  {
    if (!lhead)
    {
      lhead = new ListTools::NamedList<int>{data, nullptr, nullptr};
      lIt.setCurr(lhead);
    }
    else
    {
      lIt.insert(nullptr, data);
      ++lIt;
    }
  }
  lIt.setCurr(lhead);
  printNames(lIt);
  std::cout << "\n";
  printSeq(lIt, embedIt, 0);
}
