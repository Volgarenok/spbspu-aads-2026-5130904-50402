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
  alisov::BiList< std::pair< std::string, alisov::BiList< int > > > sequences{};

  {
    std::pair< std::string, alisov::BiList< int > > pair;
    std::string name;
    alisov::BiList< int > seq{};

    int curr = 0;
    if (!(std::cin >> name)) {
      std::cout << 0 << '\n';
      return 0;
    }

    while (true) {
      if (!(std::cin >> curr)) {
        if (std::cin.eof()) {
          pair = {name, seq};
          sequences.pushBack(pair);
          break;
        }
        std::cin.clear();
        std::string bad;
        if (std::cin >> bad) {
          bool flag = true;
          for (char s : bad) {
            if (!std::isdigit(static_cast< unsigned char >(s))) {
              flag = false;
              break;
            }
          }
          if (flag) {
            std::cerr << "Incorrect input\n";
            return 1;
          }
          pair = {name, seq};
          sequences.pushBack(pair);
          seq.clear();
          name = bad;
        }
      } else {
        seq.pushBack(curr);
      }
    }
  }
  bool firstName = true;
  for (const auto &pair : sequences) {
    if (!firstName) {
      std::cout << ' ';
    }
    std::cout << pair.first;
    firstName = false;
  }
  std::cout << '\n';

  auto listOfSum = alisov::BiList< int >();
  alisov::BiList< alisov::BLEnds > iterators;
  for (auto &pair : sequences) {
    iterators.pushBack({pair.second.begin(), pair.second.end()});
  }

  while (true) {
    bool flag = true;
    int sum = 0;
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
