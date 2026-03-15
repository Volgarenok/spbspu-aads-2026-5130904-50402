#include <iostream>
#include <string>
#include <utility>
#include <limits>
#include <stdexcept>
#include <cctype>

#include "list.hpp"

int main()
{
  try
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

        if (!(std::cin >> value))
        {
          throw std::overflow_error("overflow");
        }
        numTail = numbers.insertAfter(numTail, value);
      }

      std::pair< std::string, matveev::List< size_t > > seq(name, numbers);
      tail = sequences.insertAfter(tail, seq);
    }

    if (sequences.begin() == sequences.end())
    {
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

    while (more)
    {
      more = false;
      size_t rowSum = 0;
      bool firstInRow = true;

      for (auto seqIt = sequences.begin(); seqIt != sequences.end(); ++seqIt)
      {
        auto &list = seqIt->second;
        auto valIt = list.begin();

        for (size_t i = 0; i < rowIndex && valIt != list.end(); ++i)
        {
          ++valIt;
        }

        if (valIt != list.end())
        {
          size_t v = *valIt;
          if (!firstInRow)
          {
            if (v > (std::numeric_limits<size_t>::max() - rowSum))
            {
              throw std::overflow_error("overflow");
            }
            std::cout << " ";
          }

          std::cout << v;
          rowSum += v;
          firstInRow = false;
          more = true;
        }
      }

      if (more)
      {
        sumTail = sums.insertAfter(sumTail, rowSum);
        std::cout << "\n";
        rowIndex++;
      }
    }

    auto sit = sums.begin();
    if (sit != sums.end())
    {
      std::cout << *sit;
      for (++sit; sit != sums.end(); ++sit)
      {
        std::cout << " " << *sit;
      }
      std::cout << "\n";
    }
    else
    {
      std::cout << "0\n";
    }

    return 0;
  }
  catch (const std::overflow_error &)
  {
    std::cerr << "overflow\n";
    return 1;
  }
}
