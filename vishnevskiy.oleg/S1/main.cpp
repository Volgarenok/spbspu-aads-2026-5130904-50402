#include "ListImpl.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <cctype>
#include <limits>

bool hasDepth(vishnevskiy::LIter<size_t> em, size_t depth)
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

void printNames(vishnevskiy::NamedLIter<size_t> lt)
{
  while (lt.hasNext())
  {
    std::cout << lt.getName() << " ";
    ++lt;
  }
  std::cout << lt.getName();
}

bool printEmbed(vishnevskiy::LIter<size_t> em, size_t depth, size_t& sm, bool& overflow)
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
    if (em.value() > std::numeric_limits<size_t>::max() - sm)
    {
      overflow = true;
    }
    if (!overflow)
    {
      sm += em.value();
    }
    return true;
  }
  return false;
}

void printSeq(vishnevskiy::NamedLIter<size_t> lt, size_t depth, size_t* sums, size_t index, bool& overflow)
{
  bool f = false;
  size_t sm = 0;
  vishnevskiy::NamedLIter<size_t> currIt = lt;
  while (currIt.curr)
  {
    if (currIt.value())
    {
      vishnevskiy::LIter<size_t> temp(currIt.value());
      if (hasDepth(temp, depth))
      {
        if (f)
        {
          std::cout << " ";
        }

        f = printEmbed(temp, depth, sm, overflow);
      }
    }
    ++currIt;
  }
  if (f)
  {
    std::cout << "\n";
    sums[index] = sm;
    printSeq(lt, depth+1, sums, index+1, overflow);
  }
}

void cleanup(vishnevskiy::NamedLIter<size_t>& lt, vishnevskiy::LIter<size_t>& em, vishnevskiy::NamedList<size_t>* h)
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
  vishnevskiy::NamedList<size_t>* lhead = nullptr;
  vishnevskiy::NamedLIter<size_t> lIt(lhead);
  vishnevskiy::LIter<size_t> embedIt(nullptr);
  std::string name;
  size_t lSize = 0;
  size_t cSize = 0;
  bool overflow = false, overflow_sum = false;
  while (std::cin)
  {
    std::cin >> std::ws;
    int c = std::cin.peek();
    if (c != EOF)
    {
      if (std::isdigit(c))
      {
        size_t number = 0;
        std::cin >> number;
        if (std::cin.fail())
        {
          std::cerr << "Overflow\n";
          overflow = true;
          std::cin.clear();
          continue;
        }
        if (!lIt.value())
        {
          try
          {
            vishnevskiy::List<size_t>* embed = new vishnevskiy::List<size_t>{number, nullptr};
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
            lhead = new vishnevskiy::NamedList<size_t>{name, nullptr, nullptr};
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

  if (!lhead)
  {
    std::cout << 0 << "\n";
    return 0;
  }

  lIt.setCurr(lhead);
  size_t* sums = nullptr;
  try
  {
    sums = new size_t[lSize];
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
  printSeq(lIt, 0, sums, 0, overflow_sum);
  if (overflow_sum)
  {
    std::cerr << "Overflow\n";
    cleanup(lIt, embedIt, lhead);
    delete[] sums;
    return 1;
  }
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
  if (overflow)
  {
    return 1;
  }
  return 0;
}
