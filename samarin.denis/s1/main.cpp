#include <iostream>
#include <string>
#include <limits>
#include "list.hpp"

template< class T >
List< std::pair< std::string, List< T > > > readInput(std::istream& in) {
  List< std::pair< std::string, List< T > > > res;
  LIter< std::pair< std::string, List< T > > > res_tail = res.before_begin();

  std::string name;
  while (in >> name) {

    List< T > sublist;
    LIter< T > tail = sublist.before_begin();
    T val;
    while (in >> val) {
      tail = sublist.insert_after(tail, val);
    }

    if (!in.eof()) {
      in.clear();
    }

    res_tail = res.insert_after(res_tail, std::make_pair(name, std::move(sublist)));
  }

  return res;
}

int main() {
  auto data = readInput< int >(std::cin);

  if (data.empty()) {
    std::cout << "0\n";
    return 0;
  }

  // вывод имён
  bool first = true;
  for (auto it = data.cbegin(); it != data.cend(); ++it) {
    if (!first) {
      std::cout << " ";
    }
    std::cout << it->first;
    first = false;
  }
  std::cout << "\n";

  // итераторы текущих позиций в каждом подсписке
  List< LCIter< int > > positions;
  {
    LIter< LCIter< int > > pos_tail = positions.before_begin();
    for (auto it = data.cbegin(); it != data.cend(); ++it) {
      pos_tail = positions.insert_after(pos_tail, it->second.cbegin());
    }
  }

  // суммы зип-последовательностей
  List< int > sums;
  LIter< int > sums_tail = sums.before_begin();

  LCIter< int > end_marker;
  bool any_remaining = true;
  while (any_remaining) {
    any_remaining = false;
    bool line_started = false;
    int sum = 0;

    for (auto pit = positions.begin(); pit != positions.end(); ++pit) {
      if (*pit != end_marker) {
        any_remaining = true;
        int val = **pit;
        if (line_started) {
          std::cout << " ";
        }
        std::cout << val;
        line_started = true;

        if (val > 0 && sum > std::numeric_limits< int >::max() - val) {
          std::cerr << "overflow" << "\n";
          return 1;
        }

        sum += val;
        ++(*pit);
      }
    }

    if (any_remaining) {
      std::cout << "\n";
      sums_tail = sums.insert_after(sums_tail, sum);
    }
  }

  // вывод сумм
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
