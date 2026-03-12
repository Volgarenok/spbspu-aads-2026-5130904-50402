#include <iostream>
#include <string>
#include "bilist.hpp"

namespace shirokov
{
  struct BLEnds
  {
    shirokov::BLIter< size_t > curr;
    shirokov::BLIter< size_t > end;
  };
}

int main()
{
  shirokov::BiList< std::pair< std::string, shirokov::BiList< size_t > > > sequences{};

  std::string name;

  while (std::cin >> name)
  {
    shirokov::BiList< size_t > nums;
    size_t num;
    while (std::cin >> num)
    {
      nums.push_back(num);
    }
    std::cin.clear();
    sequences.push_back({name, nums});
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
