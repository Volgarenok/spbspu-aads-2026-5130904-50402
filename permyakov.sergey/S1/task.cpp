#include <iostream>
#include "task.hpp"

namespace permyakov
{
  using list_t = List < std::pair < std::string, List < size_t > > >;

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
      list.push_front(std::pair < std::string, List < size_t > > (title, numbers));
    }
  }

  /*void output(std::ostream & out, const list_t & list)
  {
    return;
  }*/
}
