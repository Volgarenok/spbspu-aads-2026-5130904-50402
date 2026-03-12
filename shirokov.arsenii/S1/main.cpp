#include <iostream>
#include <limits>
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

  shirokov::BiList< shirokov::BiList< size_t > > rows;
  shirokov::BiList< size_t > listOfSum{};
  shirokov::BiList< shirokov::BLEnds > iterators;

  for (auto& pair : sequences)
  {
    iterators.push_back({pair.second.begin(), pair.second.end()});
  }

  while (true)
  {
    bool flag = true;
    size_t sum = 0;
    shirokov::BiList< size_t > row;
    for (shirokov::BLEnds& ends : iterators)
    {
      if (ends.curr == ends.end)
      {
        continue;
      }
      size_t value = *ends.curr;
      if (sum > std::numeric_limits< size_t >::max() - value)
      {
        std::cerr << "Overflow error\n";
        return 1;
      }
      sum += value;
      row.push_back(value);
      ++ends.curr;
      flag = false;
    }
    if (flag)
    {
      break;
    }
    rows.push_back(row);
    listOfSum.push_back(sum);
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

  for (auto& row : rows)
  {
    bool first = true;
    for (size_t v : row)
    {
      if (!first)
      {
        std::cout << ' ';
      }
      std::cout << v;
      first = false;
    }
    std::cout << '\n';
  }

  if (listOfSum.empty())
  {
    std::cout << 0 << '\n';
    return 0;
  }

  bool first = true;
  for (size_t s : listOfSum)
  {
    if (!first)
    {
      std::cout << ' ';
    }
    std::cout << s;
    first = false;
  }
  std::cout << '\n';
}
