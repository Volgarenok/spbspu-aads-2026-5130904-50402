#include "bilist.hpp"
#include <iostream>
#include <limits>
#include <utility>

int main()
{
  shirokov::BiList< std::pair< std::string, shirokov::BiList< size_t > > > seq{};

  // std::cout << "FIRST\n";
  std::string name;
  while (std::cin >> name)
  {
    shirokov::BiList< size_t > nums{};
    size_t num;
    while (std::cin >> num)
    {
      nums.push_back(num);
    }
    std::cin.clear();
    seq.push_back({name, nums});
  }

  // std::cout << "SECOND\n";
  for (auto it = seq.begin(); it != seq.end(); ++it)
  {
    if (it != seq.begin())
    {
      std::cout << ' ';
    }
    std::cout << (*it).first;
  }
  if (!seq.empty())
  {
    std::cout << '\n';
  }

  // std::cout << "THIRD\n";
  shirokov::BiList< std::pair< shirokov::BLIter< size_t >, shirokov::BLIter< size_t > > > iterators{};
  for (auto it = seq.begin(); it != seq.end(); ++it)
  {
    iterators.push_back({(*it).second.begin(), (*it).second.end()});
  }

  // std::cout << "FOURTH\n";
  while (true)
  {
    bool areValid = false;
    bool isFirst = true;
    for (auto& ends : iterators)
    {
      if (ends.first != ends.second)
      {
        if (!isFirst)
        {
          std::cout << ' ';
        }
        std::cout << *ends.first;
        isFirst = false;
        ++ends.first;
        areValid = true;
      }
    }
    if (!areValid)
    {
      break;
    }
    std::cout << '\n';
  }

  // std::cout << "FIFTH\n";
  iterators.clear();
  for (auto it = seq.begin(); it != seq.end(); ++it)
  {
    iterators.push_back({(*it).second.begin(), (*it).second.end()});
  }

  // std::cout << "SIXTH\n";
  shirokov::BiList< size_t > sums{};
  while (true)
  {
    bool areValid = false;
    size_t sum = 0;
    for (auto& ends : iterators)
    {
      if (ends.first != ends.second)
      {
        if (sum > std::numeric_limits< size_t >::max() - *ends.first)
        {
          std::cerr << "Overflow error\n";
          return 1;
        }
        sum += *ends.first;
        ++ends.first;
        areValid = true;
      }
    }

    if (!areValid)
    {
      break;
    }
    sums.push_back(sum);
  }

  // std::cout << "SEVENTH\n";
  bool isFirst = true;
  for (size_t s : sums)
  {
    if (!isFirst)
    {
      std::cout << ' ';
    }
    std::cout << s;
    isFirst = false;
  }
  std::cout << '\n';
}
