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
    std::cin >> name;

    while (true) {
      if (!(std::cin >> curr)) {
        if (std::cin.eof()) {
          pair = {name, seq};
          sequences.push_back(pair);
          break;
        }
        std::cin.clear();
        std::string bad;
        if (std::cin >> bad) {
          bool flag = true;
          for (char s : bad) {
            if (!std::isdigit(s)) {
              flag = false;
              break;
            }
          }
          if (!flag) {
            std::cerr << "Incorrect input\n";
            return 1;
          }
          pair = {name, seq};
          sequences.push_back(pair);
          seq.clear();
          name = bad;
        }
      } else {
        seq.push_back(curr);
      }
    }
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
    iterators.push_back({pair.second.begin(), pair.second.end()});
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
    listOfSum.push_back(sum);
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
