#include <iostream>
#include <limits>
#include <utility>
#include "list.hpp"

int main()
{
  alisov::BiList< std::pair< std::string, alisov::BiList< size_t > > > sequences{};

  std::string sequenceName;
  while (std::cin >> sequenceName) {
    alisov::BiList< size_t > values{};
    size_t value;
    while (std::cin >> value) {
      values.pushBack(value);
    }
    std::cin.clear();
    sequences.pushBack({sequenceName, values});
  }

  for (auto sequenceIt = sequences.begin(); sequenceIt != sequences.end(); ++sequenceIt) {
    if (sequenceIt != sequences.begin()) {
      std::cout << ' ';
    }
    std::cout << (*sequenceIt).first;
  }
  if (!sequences.empty()) {
    std::cout << '\n';
  }

  alisov::BiList< std::pair< alisov::BLIter< size_t >, alisov::BLIter< size_t > > > sequenceIters{};
  for (auto sequenceIt = sequences.begin(); sequenceIt != sequences.end(); ++sequenceIt) {
    sequenceIters.pushBack({(*sequenceIt).second.begin(), (*sequenceIt).second.end()});
  }

  while (true) {
    bool hasValues = false;
    bool isFirstValue = true;
    for (auto &iteratorPair : sequenceIters) {
      if (iteratorPair.first != iteratorPair.second) {
        if (!isFirstValue) {
          std::cout << ' ';
        }
        std::cout << *iteratorPair.first;
        isFirstValue = false;
        ++iteratorPair.first;
        hasValues = true;
      }
    }
    if (!hasValues) {
      break;
    }
    std::cout << '\n';
  }

  sequenceIters.clear();
  for (auto sequenceIt = sequences.begin(); sequenceIt != sequences.end(); ++sequenceIt) {
    sequenceIters.pushBack({(*sequenceIt).second.begin(), (*sequenceIt).second.end()});
  }

  alisov::BiList< size_t > rowSums{};
  while (true) {
    bool hasValues = false;
    size_t currentSum = 0;
    for (auto &iteratorPair : sequenceIters) {
      if (iteratorPair.first != iteratorPair.second) {
        if (currentSum > std::numeric_limits< size_t >::max() - *iteratorPair.first) {
          std::cerr << "Overflow error\n";
          return 1;
        }
        currentSum += *iteratorPair.first;
        ++iteratorPair.first;
        hasValues = true;
      }
    }

    if (!hasValues) {
      break;
    }
    rowSums.pushBack(currentSum);
  }

  if (rowSums.empty()) {
    std::cout << 0 << '\n';
    return 0;
  }

  bool isFirstSum = true;
  for (size_t sumValue : rowSums) {
    if (!isFirstSum) {
      std::cout << ' ';
    }
    std::cout << sumValue;
    isFirstSum = false;
  }
  std::cout << '\n';
}
