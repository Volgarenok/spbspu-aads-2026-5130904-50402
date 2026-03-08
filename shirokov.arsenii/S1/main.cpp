#include <iostream>
#include "bilist.hpp"

namespace shirokov
{
  struct BLEnds
  {
    shirokov::BLIter< int > curr;
    shirokov::BLIter< int > end;
  };
}

int main()
{
  shirokov::BiList< std::pair< std::string, shirokov::BiList< int > > > sequences{};

  {
    std::pair< std::string, shirokov::BiList< int > > pair;
    std::string name;
    shirokov::BiList< int > seq{};

    int curr = 0;
    std::cin >> name;

    while (true)
    {
      if (!(std::cin >> curr))
      {
        if (std::cin.eof())
        {
          pair = {name, seq};
          sequences.push_back(pair);
          break;
        }
        std::cin.clear();
        std::string bad;
        if (std::cin >> bad)
        {
          pair = {name, seq};
          sequences.push_back(pair);
          seq.clear();
          name = bad;
        }
      }
      else
      {
        seq.push_back(curr);
      }
    }
  }

  for (const auto& pair : sequences)
  {
    if (&pair == &sequences.back())
    {
      std::cout << pair.first << '\n';
    }
    else
    {
      std::cout << pair.first << ' ';
    }
  }

  shirokov::BiList< int > listOfSum{};
  shirokov::BiList< shirokov::BLEnds > iterators;
  for (auto& pair : sequences)
  {
    iterators.push_back({pair.second.begin(), pair.second.end()});
  }

  while (true)
  {
    bool flag = true;
    int sum = 0;
    for (shirokov::BLEnds& ends : iterators)
    {
      if (ends.curr == ends.end)
      {
        continue;
      }
      if (&ends != &iterators.front())
      {
        std::cout << ' ';
      }
      std::cout << *ends.curr;
      sum += *ends.curr;
      ++ends.curr;
      flag = false;
    }
    if (flag)
    {
      break;
    }
    std::cout << '\n';
    listOfSum.push_back(sum);
  }

  if (listOfSum.empty())
  {
    std::cout << 0 << '\n';
  }
  for (int s : listOfSum)
  {
    if (s != listOfSum.front())
    {
      std::cout << ' ';
    }
    std::cout << s;
  }
  std::cout << '\n';
}
