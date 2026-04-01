#include <iostream>
#include "list.hpp"

namespace alisov
{
  struct BLEnds
  {
    alisov::BLIter< int > curr;
    alisov::BLIter< int > end;
  };
}

template < class T >
bool hex(T t);

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
}
