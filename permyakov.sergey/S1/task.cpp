#include <iostream>
#include "task.hpp"

namespace permyakov
{
  using pair_t = std::pair < std::string, List < size_t > >;

  void input(std::istream & in, List < pair_t > & list)
  {
    std::string title;
    List < size_t > numbers;
    if(in >> title) {
      inputNum(in, numbers);
    }
    pair_t pairL(title, numbers);
    list.push_front(pairL);
    LIter < pair_t > iterList = list.begin();
    while(in >> title) {
      inputNum(in, numbers);
      pair_t pairL(title, numbers);
      list.insert_after(iterList, pairL);
      ++iterList;
    }
  }

  void inputNum(std::istream & in, List < size_t > & numbers)
  {
    numbers.clear();
    size_t num;
    if(in >> num){
      numbers.push_front(num);
    } else {
      in.clear();
      return;
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
    idList.push_front(0);
    LIter < size_t > iterIdList = idList.begin();
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
      idList.insert_after(iterIdList, *iIterList);
      ++iterIdList;
    }
    idList.pop_front();
    return idList;
  }

  size_t sumList(const List < size_t > list)
  {
    size_t result = 0;
    LCIter < size_t > iterList = list.beginC();
    for(size_t i = 0; i < list.size(); ++i) {
      result += *iterList;
      ++iterList;
    }
    return result;
  }

  void outputName(std::ostream & out, const List < pair_t > & list)
  {
    if(list.isEmpty()) {
      return;
    }
    LCIter < pair_t > iterList = list.beginC();
    for(size_t i = 0; i < list.size(); ++i) {
      out << (*iterList).first << ' ';
      ++iterList;
    }
    out << '\n';
  }

  void outputNum(std::ostream & out, const List < size_t > & list)
  {
    if(list.isEmpty()) {
      return;
    }
    LCIter < size_t > iterList = list.beginC();
    for(size_t i = 0; i < list.size(); ++i) {
      out << *iterList << ' ';
      ++iterList;
    }
    out <<'\n';
  }
}
