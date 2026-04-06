#include <iostream>
#include <limits>
#include <utility>
#include "list.hpp"

int main()
{
  alisov::BiList< std::pair< std::string, alisov::BiList< size_t > > > seq{};

  std::string name;
  if (!(std::cin >> name)) {
    std::cout << 0 << '\n';
    return 0;
  }

  while (true) {
    alisov::BiList< size_t > nums{};
    size_t num;
    while (std::cin >> num) {
      nums.pushBack(num);
    }

    seq.pushBack({name, nums});

    if (std::cin.eof()) {
      break;
    }

    std::cin.clear();
    if (!(std::cin >> name)) {
      break;
    }
  }

  alisov::BiList< std::pair< alisov::BLIter< size_t >, alisov::BLIter< size_t > > > iterators{};
  for (auto it = seq.begin(); it != seq.end(); ++it) {
    iterators.pushBack({(*it).second.begin(), (*it).second.end()});
  }

  alisov::BiList< size_t > sums{};
  while (true) {
    bool areValid = false;
    size_t sum = 0;
    for (auto &ends : iterators) {
      if (ends.first != ends.second) {
        if (sum > std::numeric_limits< size_t >::max() - *ends.first) {
          std::cerr << "Overflow error\n";
          return 1;
        }
        sum += *ends.first;
        ++ends.first;
        areValid = true;
      }
    }

    if (!areValid) {
      break;
    }
    sums.pushBack(sum);
  }

  for (auto it = seq.begin(); it != seq.end(); ++it) {
    if (it != seq.begin()) {
      std::cout << ' ';
    }
    std::cout << (*it).first;
  }
  if (!seq.empty()) {
    std::cout << '\n';
  }

  iterators.clear();
  for (auto it = seq.begin(); it != seq.end(); ++it) {
    iterators.pushBack({(*it).second.begin(), (*it).second.end()});
  }

  while (true) {
    bool areValid = false;
    bool isFirst = true;
    for (auto &ends : iterators) {
      if (ends.first != ends.second) {
        if (!isFirst) {
          std::cout << ' ';
        }
        std::cout << *ends.first;
        isFirst = false;
        ++ends.first;
        areValid = true;
      }
    }
    if (!areValid) {
      break;
    }
    std::cout << '\n';
  }

  if (sums.empty()) {
    std::cout << 0 << '\n';
    return 0;
  }

  bool isFirst = true;
  for (size_t s : sums) {
    if (!isFirst) {
      std::cout << ' ';
    }
    std::cout << s;
    isFirst = false;
  }
  std::cout << '\n';

  return 0;
}
