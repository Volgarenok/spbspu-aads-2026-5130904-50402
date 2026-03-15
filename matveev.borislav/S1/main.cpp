#include <iostream>
#include <string>
#include <utility>
#include <limits>
#include <climits>
#include <cctype>

#include "list.hpp"

int main()
{
  matveev::List< std::pair< std::string, matveev::List< unsigned long long > > > sequences;
  auto tail = sequences.beforeBegin();
  std::string name;

  while (std::cin >> name)
  {
    matveev::List< unsigned long long > numbers;
    auto numTail = numbers.beforeBegin();
    unsigned long long value;

    while (std::cin.peek() != '\n' && std::cin.peek() != EOF)
    {
      if (std::isspace(std::cin.peek()))
      {
        std::cin.ignore();
        continue;
      }

      if (!(std::cin >> value))
      {
        std::cerr << "Error\n";
        return 1;
      }
      numTail = numbers.insertAfter(numTail, value);
    }

    std::pair< std::string, matveev::List< unsigned long long > > seq(name, numbers);
    tail = sequences.insertAfter(tail, seq);
  }

  if (sequences.begin() == sequences.end())
  {
    return 0;
  }

  matveev::List< unsigned long long > sums;
  auto sumTail = sums.beforeBegin();
  bool more = true;
  size_t rowIndex = 0;

  while (more)
  {
    more = false;
    unsigned long long rowSum = 0;
    bool hasValueInRow = false;

    for (auto it = sequences.begin(); it != sequences.end(); ++it)
    {
      auto &list = it->second;
      auto valIt = list.begin();

      for (size_t i = 0; i < rowIndex && valIt != list.end(); ++i)
      {
        ++valIt;
      }

      if (valIt != list.end())
      {
        unsigned long long v = *valIt;
        if (hasValueInRow)
        {
          if (v > (ULLONG_MAX - rowSum))
          {
            std::cerr << "Error\n";
            return 1;
          }
          rowSum += v;
        }
        else
        {
          rowSum = v;
          hasValueInRow = true;
        }
        more = true;
      }
    }

    if (more)
    {
      sumTail = sums.insertAfter(sumTail, rowSum);
      rowIndex++;
    }
  }

  auto it = sequences.begin();
  std::cout << it->first;
  for (++it; it != sequences.end(); ++it)
  {
    std::cout << " " << it->first;
  }
  std::cout << "\n";

  for (size_t i = 0; i < rowIndex; ++i)
  {
    bool firstInRow = true;
    for (auto sit = sequences.begin(); sit != sequences.end(); ++sit)
    {
      auto valIt = sit->second.begin();
      for (size_t j = 0; j < i && valIt != sit->second.end(); ++j)
      {
        ++valIt;
      }

      if (valIt != sit->second.end())
      {
        if (!firstInRow)
        {
          std::cout << " ";
        }
        std::cout << *valIt;
        firstInRow = false;
      }
    }
    std::cout << "\n";
  }

  if (sums.begin() == sums.end())
  {
    std::cout << 0 << "\n";
  }
  else
  {
    auto sit = sums.begin();
    std::cout << *sit;
    for (++sit; sit != sums.end(); ++sit)
    {
      std::cout << " " << *sit;
    }
    std::cout << "\n";
  }

  return 0;
}
