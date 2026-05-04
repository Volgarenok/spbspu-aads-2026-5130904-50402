#include "List.hpp"
#include <iostream>
#include <limits>
#include <string>

int main()
{
  saldaev::List< std::pair< std::string, saldaev::List< size_t > > > outer;

  std::string name = "";
  size_t num = 0;
  while (std::cin >> name) {
    saldaev::List< size_t > numList;
    while (std::cin >> num) {
      numList.pushBack(num);
    }
    outer.pushBack({name, std::move(numList)});

    if (!std::cin.eof()) {
      std::cin.clear();
    }
  }

  if (outer.begin() == outer.end()) {
    std::cout << "0\n";
    return 0;
  }

  typename saldaev::List< std::pair< std::string, saldaev::List< size_t > > >::LIter outerIt = outer.begin();
  saldaev::List< typename saldaev::List< size_t >::LIter > iters;
  while (outerIt != outer.end()) {
    iters.pushBack(outerIt->second.begin());
    ++outerIt;
  }

  outerIt = outer.begin();
  bool first = true;
  while (outerIt != outer.end()) {
    if (!first) {
      std::cout << ' ';
    }
    std::cout << outerIt->first;
    first = false;
    ++outerIt;
  }

  bool any_left = true;
  while (any_left) {
    std::cout << '\n';
    any_left = false;
    typename saldaev::List< typename saldaev::List< size_t >::LIter >::LIter iterIt = iters.begin();

    first = true;
    while (iterIt != iters.end()) {
      typename saldaev::List< size_t >::LIter &innerIt = *iterIt;
      if (innerIt != outer.begin()->second.end()) {
        if (!first) {
          std::cout << ' ';
        }
        std::cout << *innerIt;
        ++innerIt;
        any_left = true;
        first = false;
      }
      ++iterIt;
    }
  }

  outerIt = outer.begin();
  typename saldaev::List< typename saldaev::List< size_t >::LIter >::LIter iterIt = iters.begin();
  while (outerIt != outer.end()) {
    *iterIt = outerIt->second.begin();
    ++outerIt;
    ++iterIt;
  }

  saldaev::List< size_t > sums;
  try {
    for (size_t i = 0; i < iters.size(); ++i) {
      sums.pushFront(0);
    }
    typename saldaev::List< size_t >::LIter sumsIt = sums.begin();
    any_left = true;
    while (any_left) {
      any_left = false;
      typename saldaev::List< typename saldaev::List< size_t >::LIter >::LIter iterIt = iters.begin();

      while (iterIt != iters.end()) {
        typename saldaev::List< size_t >::LIter &innerIt = *iterIt;
        if (innerIt != outer.begin()->second.end()) {
          size_t num = *innerIt;
          if (std::numeric_limits< size_t >::max() - num < *sumsIt) {
            throw std::overflow_error("overflow_error");
          }
          *sumsIt += *innerIt;
          ++innerIt;
          any_left = true;
          first = false;
        }
        ++iterIt;
      }
      ++sumsIt;
    }
  } catch (...) {
    return 1;
  }

  typename saldaev::List< size_t >::LIter sumsIt = sums.begin();
  first = true;
  while (sumsIt != sums.end()) {
    if (!first) {
      std::cout << ' ';
    }
    std::cout << *sumsIt;
    first = false;
    ++sumsIt;
  }
  std::cout << '\n';
}
