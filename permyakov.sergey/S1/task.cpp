#include <iostream>
#include "task.hpp"

namespace permyakov
{
  using pair_t = std::pair < std::string, List < size_t > >;
  using list_t = List < pair_t >;

  void input(std::istream & in, list_t & list)
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
      pair_t pairL(title, numbers);
      list.push_front(pairL);
    }
  }

  /*void output(std::ostream & out, const list_t & list)
  {
    return;
  }*/
}
