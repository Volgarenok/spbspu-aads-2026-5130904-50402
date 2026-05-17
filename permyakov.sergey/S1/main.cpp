#include <iostream>
#include "task.hpp"

int main()
{
  namespace per = permyakov;
  using pair_t = std::pair < std::string, per::List < size_t > >;
  per::List < pair_t > bgList;
  per::input(std::cin, bgList);
  per::outputName (std::cout, bgList);
  per::LIter < pair_t > iterList = bgList.begin();
  size_t max_size = (*iterList).second.size();
  for(size_t i = 1; i < bgList.size(); ++i) {
    ++iterList;
    size_t iSize = (*(iterList)).second.size();
    if(iSize > max_size) {
      max_size = iSize;
    }
  }
  for(size_t i = 0; i < max_size; ++i) {
    per::List < size_t > outList = per::idList(bgList, i);
    per::outputNum (std::cout, outList);
  }
}