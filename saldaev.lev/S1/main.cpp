#include "List.hpp"
#include <iostream>
#include <string>

int main()
{
  saldaev::List< std::pair< std::string, saldaev::List< size_t > > > outer;

  std::string name = "";
  size_t num = 0;
  while (std::cin >> name) {
    saldaev::List< size_t > numList;
    while (std::cin >> num) {
      numList.newTail(num);
    }
    outer.newTail({name, std::move(numList)});

    if (!std::cin.eof()) {
      std::cin.clear();
    }
  }

  if (outer.begin() == outer.end()) {
    return 0;
  }

  saldaev::LIter< std::pair< std::string, saldaev::List< size_t > > > outerIt = outer.begin();
  saldaev::List< saldaev::LIter< size_t > > iters;
  while (outerIt != outer.end()) {
    iters.newTail(outerIt.getData().second.begin());
    ++outerIt;
  }

  outerIt = outer.begin();
  bool first = true;
  while (outerIt != outer.end()) {
    if (!first) {
      std::cout << ' ';
    }
    std::cout << outerIt.getData().first;
    first = false;
    ++outerIt;
  }

  saldaev::List< size_t > sums;
  for (size_t i = 0; i < iters.getLength(); ++i) {
    sums.newHead(0);
  }
  saldaev::LIter< size_t > sumsIt = sums.begin();

  bool any_left = true;
  while (any_left) {
    std::cout << '\n';
    any_left = false;
    saldaev::LIter< saldaev::LIter< size_t > > iterIt = iters.begin();

    first = true;
    while (iterIt != iters.end()) {
      saldaev::LIter< size_t > &innerIt = iterIt.getData();
      if (innerIt.isValid()) {
        if (!first) {
          std::cout << ' ';
        }
        std::cout << innerIt.getData();
        sumsIt.getData() += innerIt.getData();
        ++innerIt;
        any_left = true;
        first = false;
      }
      ++iterIt;
    }
    ++sumsIt;
  }

  sumsIt = sums.begin();
  first = true;
  while (sumsIt != sums.end()) {
    if (!first) {
      std::cout << ' ';
    }
    std::cout << sumsIt.getData();
    first = false;
    ++sumsIt;
  }
  std::cout << '\n';
}
