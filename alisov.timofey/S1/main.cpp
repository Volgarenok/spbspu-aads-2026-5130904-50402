#include <cctype>
#include <iostream>
#include <string>
#include "list.hpp"

namespace alisov
{
  struct BLEnds
  {
    alisov::BLIter< int > curr;
    alisov::BLIter< int > end;
  };
}

int main()
{
  alisov::BiList< std::pair< std::string, alisov::BiList< size_t > > > sequences{};

  {
    std::string name;
    if (!(std::cin >> name)) {
      std::cout << 0 << '\n';
      return 0;
    }
    std::string token;

    while (std::cin >> token) {
    }

    for (const auto &pair : sequences) {
      if (&pair == &sequences.back()) {
        std::cout << pair.first << '\n';
      } else {
        std::cout << pair.first << ' ';
      }
    }

    auto listOfSum = alisov::BiList< int >();
    alisov::BiList< alisov::BLEnds > iterators;
    for (auto &pair : sequences) {
      iterators.pushBack({pair.second.begin(), pair.second.end()});
    }

    while (true) {
      bool flag = true;
      size_t sum = 0;
      for (alisov::BLEnds &ends : iterators) {
        if (ends.curr == ends.end) {
          continue;
        }
        if (&ends != &iterators.front()) {
          std::cout << ' ';
        }
        std::cout << *ends.curr;
        sum += *ends.curr;
        ++ends.curr;
        flag = false;
      }
      if (flag) {
        break;
      }
      std::cout << '\n';
      listOfSum.pushBack(sum);
    }

    if (listOfSum.empty()) {
      std::cout << 0 << '\n';
    }
    for (int s : listOfSum) {
      if (s != listOfSum.front()) {
        std::cout << ' ';
      }
      std::cout << s;
    }
    std::cout << '\n';
  }
