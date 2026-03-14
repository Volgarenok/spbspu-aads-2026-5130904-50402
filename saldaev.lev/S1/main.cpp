#include "List.hpp"
#include <iostream>
#include <string>

int main()
{
  saldaev::List< std::pair< std::string, saldaev::List< int > > > outer;

  std::string name = "";
  size_t num = 0;
  while (std::cin >> name) {
    saldaev::List< int > numList;
    while (std::cin >> num) {
      numList.newTail(num);
    }
    outer.newTail({name, std::move(numList)});

    if (!std::cin.eof()) {
      std::cin.clear();
    }
  }

  saldaev::LIter< std::pair< std::string, saldaev::List< int > > > outerIt = outer.begin();
  saldaev::List< saldaev::LIter< int > > iters;
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

  saldaev::List< int > sums;
  for (size_t i = 0; i < iters.getLength(); ++i) {
    sums.newHead(0);
  }
  saldaev::LIter< int > sumsIt = sums.begin();

  bool any_left = true;
  while (any_left) {
    std::cout << '\n';
    any_left = false;
    saldaev::LIter< saldaev::LIter< int > > iterIt = iters.begin();

    first = true;
    while (iterIt != iters.end()) {
      saldaev::LIter< int > &innerIt = iterIt.getData();
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