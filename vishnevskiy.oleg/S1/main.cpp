#include "ListImpl.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <cctype>
#include <limits>

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
  std::cout << lt.getName();
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
    std::cout << em.value();
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
        if (f)
        {
          std::cout << " ";
        }

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

void cleanup(vishnevskiy::NamedLIter<int>& lt, vishnevskiy::LIter<int>& em, vishnevskiy::NamedList<int>* h)
{
  lt.setCurr(h);
  while (lt.curr)
  {
    em.set(lt.value());
    em.clear(&em);
    ++lt;
  }
  lt.setCurr(h);
  lt.clear(&lt);
}

int main()
{
  vishnevskiy::NamedList<int>* lhead = nullptr;
  vishnevskiy::NamedLIter<int> lIt(lhead);
  vishnevskiy::LIter<int> embedIt(nullptr);
  std::string name;
  size_t lSize = 0;
  size_t cSize = 0;
  bool overflow = false;
  while (std::cin)
  {
    std::cin >> std::ws;
    int c = std::cin.peek();
    if (c != EOF)
    {
      if (std::isdigit(c))
      {
        int number = 0;
        std::cin >> number;
        if (std::cin.fail())
        {
          std::cerr << "Overflow error\n";
          std::cin.clear();
          overflow = true;
        }
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
            cleanup(lIt, embedIt, lhead);
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
        std::cin >> name;
        cSize = 0;
        if (!lhead)
        {
          try
          {
            lhead = new vishnevskiy::NamedList<int>{name, nullptr, nullptr};
          }
          catch (const std::bad_alloc& e)
          {
            std::cerr << "Bad alloc\n";
            cleanup(lIt, embedIt, lhead);
            return 1;
          }
          lIt.setCurr(lhead);
        }
        else
        {
          lIt.insert(nullptr, name);
          ++lIt;
        }
      }
    }
  }
  if (overflow)
  {
    return 1;
  }

  if (!lhead)
  {
    std::cout << 0 << "\n";
    return 0;
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
    cleanup(lIt, embedIt, lhead);
    delete[] sums;
    return 1;
  }
  printNames(lIt);
  std::cout << "\n";
  if (!embedIt.curr)
  {
    std::cout << 0 << "\n";
    cleanup(lIt, embedIt, lhead);
    delete[] sums;
    return 0;
  }
  printSeq(lIt, 0, sums, 0);
  for (size_t i = 0; i < lSize; i++)
  {
    std::cout << sums[i];
    if (i != lSize - 1)
    {
      std::cout << " ";
    }
  }
  std::cout << "\n";
  delete[] sums;
  cleanup(lIt, embedIt, lhead);
  return 0;
}
