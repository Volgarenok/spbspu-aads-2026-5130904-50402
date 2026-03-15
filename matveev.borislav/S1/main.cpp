#include <iostream>
#include <string>
#include <utility>
#include <climits>

#include "list.hpp"

int main()
{
  matveev::List< std::pair< std::string, matveev::List< int > > > sequences;
  auto tail = sequences.beforeBegin();
  std::string name;

  while (std::cin >> name)
  {
    matveev::List< int > numbers;
    auto numTail = numbers.beforeBegin();
    int value;

    while (true)
    {
      int c = std::cin.peek();

      if (c == '\n' || c == EOF)
      {
        break;
      }

      if (!(std::cin >> value))
      {
        std::cerr << "Error number is overflow\n";
        return 1;
      }

      numTail = numbers.insertAfter(numTail, value);
    }

    std::pair< std::string, matveev::List< int > > seq(name, numbers);
    tail = sequences.insertAfter(tail, seq);
  }

  if (sequences.begin() == sequences.end())
  {
    std::cout << 0 << "\n";
    return 0;
  }

  for (auto it = sequences.begin(); it != sequences.end(); ++it)
  {
    std::cout << it->first << " ";
  }
  std::cout << "\n";

  matveev::List< long long > sums;
  auto sumTail = sums.beforeBegin();

  bool more = true;

  while (more)
  {
    more = false;
    long long rowSum = 0;

    for (auto it = sequences.begin(); it != sequences.end(); ++it)
    {
      auto &list = it->second;
      auto iter = list.begin();

      if (iter != list.end())
      {
        int v = *iter;

        if (rowSum > LLONG_MAX - v)
        {
          std::cerr << "Error: overflow\n";
          return 1;
        }

        std::cout << v << " ";
        rowSum += v;

        list.removeFront();
        more = true;
      }
    }

    if (more)
    {
      sumTail = sums.insertAfter(sumTail, rowSum);
      std::cout << "\n";
    }
  }

  for (auto it = sums.begin(); it != sums.end(); ++it)
  {
    std::cout << *it << " ";
  }

  std::cout << "\n";
  return 0;
}
