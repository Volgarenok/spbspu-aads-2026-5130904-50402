#include <iostream>
#include "task.hpp"

namespace permyakov
{
  using pair_t = std::pair < std::string, List < size_t > >;

  void input(std::istream & in, List < std::pair < std::string, List < size_t > > > & list)
  {
    std::string title;
    while(in >> title) {
      List < size_t > numbers;
      size_t num;
      if(in >> num){
        numbers.push_front(num);
      }
      LIter < size_t > iter = numbers.begin();
      while(in >> num) {
        numbers.insert_after(iter, num);
        ++iter;
      }
      in.clear();
      std::pair < std::string, List < size_t > > pairL(title, numbers);
      list.push_front(pairL);
    }
  }

  void outputName(std::ostream & out, const List < pair_t > & list)
  {
    if(list.isEmpty()) {
      return;
    }
    LCIter < pair_t > iter = list.beginC();
    pair_t pairIt = *iter;
    out << pairIt.first;
    for(size_t i = 1; i < list.size(); ++i) {
      pairIt = *(++iter);
      out << ' ' << pairIt.first;
    }
    out << '\n';
  }

  void outputNum(std::ostream & out, const List < size_t > & list)
  {
    if(list.isEmpty()) {
      return;
    }
    LCIter < size_t > iter = list.beginC();
    out << *iter;
    for(size_t i = 1; i < list.size(); ++i) {
      ++iter;
      out << ' ' << *iter;
    }
    out << '\n';
  }
}
