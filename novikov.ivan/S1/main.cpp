#include "list.hpp"
#include <iostream>
#include <limits>
#include <utility>

int main()
{
  novikov::List< std::pair< std::string, novikov::List< size_t > > > seq{};

  std::string name;
  while (std::cin >> name)
  {
    novikov::List< size_t > nums{};
    size_t num;
    while (std::cin >> num)
    {
      nums.push_back(num);
    }
    std::cin.clear();
    seq.push_back({name, nums});
  }

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

  novikov::List< std::pair< novikov::LIter< size_t >, novikov::LIter< size_t > > > iterators{};
  for (auto it = seq.begin(); it != seq.end(); ++it)
  {
    iterators.push_back({(*it).second.begin(), (*it).second.end()});
  }

  while (true)
  {
    bool isValid = false;
    for (auto& ends : iterators)
    {
      if (ends.first != ends.second)
      {
        std::cout << *ends.first << ' ';
        ++ends.first;
        isValid = true;
      }
    }
    if (!isValid)
    {
      break;
    }
    std::cout << '\n';
  }

  iterators.clear();
  for (auto it = seq.begin(); it != seq.end(); ++it)
  {
    iterators.push_back({(*it).second.begin(), (*it).second.end()});
  }

  novikov::List< size_t > sums{};
  while (true)
  {
    bool isValid = false;
    size_t sum = 0;
    for (auto& ends : iterators)
    {
      if (ends.first != ends.second)
      {
        if (sum > std::numeric_limits< size_t >::max() - *ends.first)
        {
          std::cerr << "Overflow\n";
          return 1;
        }
        sum += *ends.first;
        ++ends.first;
        isValid = true;
      }
    }

    if (!isValid)
    {
      break;
    }
    sums.push_back(sum);
  }

  if (sums.empty())
  {
    std::cout << "0\n";
    return 0;
  }

  for (size_t s : sums)
  {
    std::cout << s << ' ';
  }
  std::cout << '\n';
}
