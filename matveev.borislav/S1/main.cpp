#include <iostream>
#include <string>
#include <utility>
#include <limits>
#include <cctype>
#include <cstdlib>

#include "list.hpp"

int main()
{
  matveev::List< std::pair< std::string, matveev::List< size_t > > > sequences;
  auto tail = sequences.beforeBegin();
  std::string name;

  while (std::cin >> name)
  {
    matveev::List< size_t > numbers;
    auto numTail = numbers.beforeBegin();
    size_t value;

    while (true)
    {
      while (std::isspace(std::cin.peek()) && std::cin.peek() != '\n')
      {
        std::cin.ignore();
      }

      int c = std::cin.peek();
      if (c == '\n' || c == EOF)
      {
        break;
      }

      std::cin >> value;
      numTail = numbers.insertAfter(numTail, value);
    }

    tail = sequences.insertAfter(tail, { name, numbers });
  }

  if (sequences.begin() == sequences.end())
  {
    std::cout << "0\n";
    return 0;
  }

  auto it = sequences.begin();
  std::cout << it->first;
  for (++it; it != sequences.end(); ++it)
  {
    std::cout << " " << it->first;
  }
  std::cout << "\n";

  matveev::List< size_t > sums;
  auto sumTail = sums.beforeBegin();

  size_t rowIndex = 0;
  bool more = true;
  bool anyRow = false;

  while (more)
  {
    more = false;
    size_t rowSum = 0;
    bool hasValue = false;

    for (auto seqIt = sequences.begin(); seqIt != sequences.end(); ++seqIt)
    {
      auto& list = seqIt->second;
      auto valIt = list.begin();

      for (size_t i = 0; i < rowIndex && valIt != list.end(); ++i)
      {
        ++valIt;
      }

      if (valIt != list.end())
      {
        size_t v = *valIt;

        if (hasValue)
        {
          std::cout << " ";
        }

        std::cout << v;

        if (v > std::numeric_limits<size_t>::max() - rowSum)
        {
          std::cerr << "Error\n";
          std::exit(1);
        }

        rowSum += v;
        hasValue = true;
        more = true;
        anyRow = true;
      }
    }

    if (more)
    {
      sumTail = sums.insertAfter(sumTail, rowSum);
      std::cout << "\n";
      rowIndex++;
    }
  }

  if (!anyRow)
  {
    std::cout << "0\n";
    return 0;
  }

  auto sit = sums.begin();
  std::cout << *sit;
  for (++sit; sit != sums.end(); ++sit)
  {
    std::cout << " " << *sit;
  }
  std::cout << "\n";

  return 0;
}

