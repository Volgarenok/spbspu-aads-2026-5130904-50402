#include <cctype>
#include <iostream>
#include <limits>
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
    bool firstValue = true;
    int sum = 0;
    for (alisov::BLEnds &ends : iterators) {
      if (ends.curr == ends.end) {
        continue;
      }
      if (!firstValue) {
        std::cout << ' ';
      }
      std::cout << *ends.curr;
      if ((*ends.curr > 0 && sum > std::numeric_limits< int >::max() - *ends.curr)
          || (*ends.curr < 0 && sum < std::numeric_limits< int >::min() - *ends.curr)) {
        std::cerr << "Incorrect input\n";
        return 1;
      }
      sum += *ends.curr;
      ++ends.curr;
      flag = false;
      firstValue = false;
    }
    if (flag) {
      break;
    }
    std::cout << '\n';
    listOfSum.pushBack(sum);
  }

  if (listOfSum.empty()) {
    std::cout << 0 << '\n';
    return 0;
  }

  bool firstSum = true;
  for (int s : listOfSum) {
    if (!firstSum) {
      std::cout << ' ';
    }
    std::cout << s;
    firstSum = false;
  }
  std::cout << '\n';
}