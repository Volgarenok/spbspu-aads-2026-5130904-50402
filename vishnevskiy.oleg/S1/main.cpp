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

bool hasDepth(vishnevskiy::LIter<int> em, size_t depth)
{
  size_t d = 0;
  while (em.curr)
  {
    if (depth == d)
    {
      return true;
    }
    d++;
    ++em;
  }
  return false;
}

void printNames(vishnevskiy::NamedLIter<int> lt)
{
  while (lt.hasNext())
  {
    std::cout << lt.getName() << " ";
    ++lt;
  }
  std::cout << lt.getName() << " ";
}

bool printEmbed(vishnevskiy::LIter<int> em, size_t depth, int& sm)
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
    sm += em.value();
    return true;
  }
  return false;
}

void printSeq(vishnevskiy::NamedLIter<int> lt, size_t depth, int* sums, size_t index)
{
  bool f = false;
  int sm = 0;
  vishnevskiy::NamedLIter<int> currIt = lt;
  while (currIt.curr)
  {
    if (currIt.value())
    {
      vishnevskiy::LIter<int> temp(currIt.value());
      if (hasDepth(temp, depth))
      {
        f = printEmbed(temp, depth, sm);
      }
    }
    ++currIt;
  }
  if (f)
  {
    std::cout << "\n";
    sums[index] = sm;
    printSeq(lt, depth+1, sums, index+1);
  }
}

int main()
{
  vishnevskiy::NamedList<int>* lhead = nullptr;
  vishnevskiy::NamedLIter<int> lIt(lhead);
  vishnevskiy::LIter<int> embedIt(nullptr);
  std::string data;
  size_t lSize = 0;
  size_t cSize = 0;
  while (std::cin >> data)
  {
    if (isNumber(data))
    {
      int number = std::stoi(data);
      if (!lIt.value())
      {
        try
        {
          vishnevskiy::List<int>* embed = new vishnevskiy::List<int>{number, nullptr};
          embedIt.set(embed);
        }
        catch (const std::bad_alloc& e)
        {
          std::cerr << "Bad alloc\n";

          return 1;
        }
        lIt.setData(embedIt.curr);
      }
      else
      {
        embedIt.insert(number);
        ++embedIt;
      }
      cSize++;
      if (cSize > lSize)
      {
        lSize = cSize;
      }
    }
    else
    {
      cSize = 0;
      if (!lhead)
      {
        lhead = new vishnevskiy::NamedList<int>{data, nullptr, nullptr};
        lIt.setCurr(lhead);
      }
      else
      {
        lIt.insert(nullptr, data);
        ++lIt;
      }
    }
  }

  if (!lhead)
  {
    std::cout << 0 << "\n";
    return 2;
  }

  lIt.setCurr(lhead);
  int* sums = nullptr;
  try
  {
    sums = new int[lSize];
  }
  catch (const std::bad_alloc& e)
  {
    std::cerr << "Bad alloc\n";
    while (lIt.curr)
    {
      embedIt.set(lIt.value());
      embedIt.clear(&embedIt);
    }
    ++lIt;
    lIt.setCurr(lhead);
    lIt.clear(&lIt);
    return 1;
  }
  printNames(lIt);
  std::cout << "\n";
  printSeq(lIt, 0, sums, 0);
  for (size_t i = 0; i < lSize; i++)
  {
    std::cout << sums[i] << " ";
  }
  std::cout << "\n";
  delete[] sums;
  lIt.setCurr(lhead);
  while (lIt.curr)
  {
    embedIt.set(lIt.value());
    embedIt.clear(&embedIt);
  }
  ++lIt;
  lIt.setCurr(lhead);
  lIt.clear(&lIt);
  return 0;
}
