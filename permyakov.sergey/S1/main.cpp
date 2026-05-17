#include <iostream>
#include "task.hpp"

int main()
{
  namespace per = permyakov;
  using pair_t = std::pair < std::string, per::List < size_t > >;

  per::List < pair_t > bgList;
  if(per::input(std::cin, bgList)) {
    std::cout << "0\n";
    return 0;
  }

  per::LIter < pair_t > iterList = bgList.begin();
  size_t max_size = (*iterList).second.size();
  for(size_t i = 1; i < bgList.size(); ++i) {
    ++iterList;
    size_t iSize = (*(iterList)).second.size();
    if(iSize > max_size) {
      max_size = iSize;
    }
  }

  if(!max_size) {
    per::outputName (std::cout, bgList);
    std::cout << "0\n";
    return 0;
  }

  try {
    per::List < per::List < size_t > > nums;
    nums.push_front(per::List < size_t > ());
    per::LIter < per::List < size_t > > iterNums = nums.begin();

    per::outputName (std::cout, bgList);
    for(size_t i = 0; i < max_size; ++i) {
      per::List < size_t > outList = per::idList(bgList, i);
      nums.insert_after(iterNums, outList);
      ++iterNums;
      per::outputNum(std::cout, outList);
    }
    nums.pop_front();
    iterNums = nums.begin();
    per::List < size_t > sums;
    sums.push_front(0);
    per::LIter < size_t > iterSums = sums.begin();
    for(size_t i = 0; i < max_size; ++i) {
      sums.insert_after(iterSums, per::sumList(*iterNums));
      ++iterNums;
      ++iterSums;
    }
    sums.pop_front();
    per::outputNum(std::cout, sums);
  } catch (const std::overflow_error & e) {
    std::cerr << e.what() << '\n';
    return 1;
  }
}
