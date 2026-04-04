#include <iostream>
#include <limits>
#include <string>
#include "list.hpp"

namespace samarin {

  template< class T >
  List< std::pair< std::string, List< T > > > readInput(std::istream& in)
  {
    List< std::pair< std::string, List< T > > > res;
    LIter< std::pair< std::string, List< T > > > resTail = res.before_begin();

    std::string name;
    while (in >> name) {
      List< T > sublist;
      LIter< T > tail = sublist.before_begin();
      T val = T();
      while (in >> val) {
        tail = sublist.insert_after(tail, val);
      }

      if (!in.eof()) {
        in.clear();
      }

      resTail = res.insert_after(resTail, std::make_pair(name, std::move(sublist)));
    }

    return res;
  }

}

int main()
{
  using namespace samarin;
  auto data = readInput< int >(std::cin);

  if (data.empty()) {
    std::cout << "0\n";
    return 0;
  }

  bool first = true;
  for (auto it = data.cbegin(); it != data.cend(); ++it) {
    if (!first) {
      std::cout << " ";
    }
    std::cout << it->first;
    first = false;
  }
  std::cout << "\n";

  List< LCIter< int > > positions;
  {
    LIter< LCIter< int > > posTail = positions.before_begin();
    for (auto it = data.cbegin(); it != data.cend(); ++it) {
      posTail = positions.insert_after(posTail, it->second.cbegin());
    }
  }

  List< int > sums;
  LIter< int > sumsTail = sums.before_begin();

  const LCIter< int > endMarker;
  bool anyRemaining = true;
  while (anyRemaining) {
    anyRemaining = false;
    bool lineStarted = false;
    int sum = 0;

    for (auto pit = positions.begin(); pit != positions.end(); ++pit) {
      if (*pit != endMarker) {
        anyRemaining = true;
        const int val = **pit;
        if (lineStarted) {
          std::cout << " ";
        }
        std::cout << val;
        lineStarted = true;

        if (val > 0 && sum > std::numeric_limits< int >::max() - val) {
          std::cerr << "overflow" << "\n";
          return 1;
        }

        sum += val;
        ++(*pit);
      }
    }

    if (anyRemaining) {
      std::cout << "\n";
      sumsTail = sums.insert_after(sumsTail, sum);
    }
  }

  if (!sums.empty()) {
    first = true;
    for (auto it = sums.cbegin(); it != sums.cend(); ++it) {
      if (!first) {
        std::cout << " ";
      }
      std::cout << *it;
      first = false;
    }
    std::cout << "\n";
  }

  return 0;
}
