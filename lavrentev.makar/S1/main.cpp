#include <cctype>
#include <iostream>
#include "List.hpp"

int main()
{
  lavrentev::List<std::pair<std::string, lavrentev::List<int>>> arr = {lavrentev::getline(std::cin)};
  lavrentev::LIter<std::pair<std::string, lavrentev::List<int>>> iterator = arr.begin();
  iterator.printNames(std::cout);

  lavrentev::List<lavrentev::LIter<int>> iters = {};
  lavrentev::List<int> sums = {};

  while (iterator != arr.end())
  {
    lavrentev::LIter<int> it = {(*iterator).second.begin()};
    iters.insert(iters.end(), it);
    ++iterator;
  }

  lavrentev::LIter<lavrentev::LIter<int>> itersIt = iters.begin();

  while (true)
  {
    int sum = 0;
    iterator = arr.begin();
    while (itersIt != iters.end())
    {
      if (*itersIt != (*iterator).second.end())
      {
        sum += **itersIt;
        std::cout << **itersIt << " ";
        ++(*itersIt);
      }
      ++itersIt;
      ++iterator;
    }
    if (sum == 0)
    {
      break;
    }
    std::cout << "\n";
    sums.insert(sums.end(), sum);
    itersIt = iters.begin();
  }
  lavrentev::LIter<int> sumIt = sums.begin();
  sumIt.printList(std::cout);
}
