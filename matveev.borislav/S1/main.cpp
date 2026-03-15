#include <iostream>
#include <string>
#include <utility>
#include <limits>
#include <stdexcept>

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
        int c = std::cin.peek();
        if (c == '\n' || c == EOF)
        {
          break;
        }

        if (!(std::cin >> value))
        {
          std::cin.clear();
          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
          break;
        }

        numTail = numbers.insertAfter(numTail, value);
      }

      std::pair< std::string, matveev::List< size_t > > seq(name, numbers);
      tail = sequences.insertAfter(tail, seq);
    }

    if (sequences.begin() == sequences.end())
    {
      std::cout << 0 << "\n";
      return 0;
    }

    matveev::List< matveev::List< size_t > > rows;
    auto rowsTail = rows.beforeBegin();

    matveev::List< size_t > sums;
    auto sumTail = sums.beforeBegin();

    bool more = true;

    while (more)
    {
      more = false;
      size_t rowSum = 0;
      matveev::List< size_t > row;
      auto rowTail = row.beforeBegin();

      for (auto it = sequences.begin(); it != sequences.end(); ++it)
      {
        auto &list = it->second;

        if (list.begin() != list.end())
        {
          size_t v = *list.begin();

          if (v > std::numeric_limits<size_t>::max() - rowSum)
          {
            throw std::overflow_error("overflow");
          }

          rowTail = row.insertAfter(rowTail, v);
          rowSum += v;

          list.removeFront();
          more = true;
        }
      }

      if (more)
      {
        rowsTail = rows.insertAfter(rowsTail, row);
        sumTail = sums.insertAfter(sumTail, rowSum);
      }
    }

    auto it = sequences.begin();
    std::cout << it->first;
    ++it;

    for (; it != sequences.end(); ++it)
    {
      std::cout << " " << it->first;
    }
    std::cout << "\n";

    for (auto r = rows.begin(); r != rows.end(); ++r)
    {
      bool first = true;
      for (auto v = r->begin(); v != r->end(); ++v)
      {
        if (!first)
        {
          std::cout << " ";
        }
        std::cout << *v;
        first = false;
      }
      std::cout << "\n";
    }

    if (sums.begin() == sums.end())
    {
      std::cout << 0 << "\n";
      return 0;
    }

    auto sit = sums.begin();
    std::cout << *sit;
    ++sit;

    for (; sit != sums.end(); ++sit)
    {
      std::cout << " " << *sit;
    }
    std::cout << "\n";

    return 0;
  }
  catch (const std::overflow_error &)
  {
    std::cerr << "Error\n";
    return 1;
  }
}
