#include <cctype>
#include <iostream>
#include <limits>
#include "List.hpp"

int main()
{
  lavrentev::List<std::pair<std::string, lavrentev::List<int>>> arr;
  try
  {
    arr = lavrentev::getline(std::cin);
  }
  catch (const std::overflow_error& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }

  /*if (arr.empty())
  {
    std::cout << "0\n";
    return 0;
  }*/

  lavrentev::LIter<std::pair<std::string, lavrentev::List<int>>> iterator = arr.begin();
  if((*iterator).first == "")
  {
    return 0;
  }
  if((*iterator).second.begin() == nullptr)
  {
    std::cout << "\n" << "0" << "\n";
    return 0;
  }
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
    bool first = true;
    while (itersIt != iters.end())
    {
      if (*itersIt != (*iterator).second.end())
      {
        int k  = **itersIt;
        if(sum > std::numeric_limits<int>::max() - k)
        {
          std::cerr << "Integer overflow";
          return 1;
        }
        sum += k;
        if(!first)
        {
          std::cout << " ";
        }
        std::cout << k;
        first = false;
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
