#include <iostream>
#include "task.hpp"

namespace permyakov
{
  using pair_t = std::pair < std::string, List < size_t > >;

  void input(std::istream & in, List < pair_t > & list)
  {
    std::string title;
    if(in >> title) {
      List < size_t > numbers;
      inputNum(in, numbers);
      pair_t pairL(title, numbers);
      list.push_front(pairL);
    }
    LIter < pair_t > iterList = list.begin();
    while(in >> title) {
      List < size_t > numbers;
      inputNum(in, numbers);
      pair_t pairL(title, numbers);
      list.insert_after(iterList, pairL);
      ++iterList;
    }
  }

  void inputNum(std::istream & in, List < size_t > & numbers)
  {
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
  }

  List < size_t > idList(const List < pair_t > list, size_t id)
  {
    List < size_t > idList;
    LCIter < pair_t > iterList = list.beginC();
    for(size_t i = 0; i < list.size(); ++i) {
      List < size_t > iList = (*(iterList)).second;
      ++iterList;
      if(iList.size() <= id) {
        continue;
      }
      LCIter < size_t > iIterList = iList.beginC();
      for(size_t j = 0; j < id; ++j) {
        ++iIterList;
      }
      idList.push_front(*iIterList);
    }
    return idList;
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
