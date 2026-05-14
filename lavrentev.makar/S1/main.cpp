#include <cctype>
#include <iostream>
#include <limits>
#include "List.hpp"

int main()
{
  lavrentev::List<std::pair<std::string, lavrentev::List<size_t>>> arr;
  try
  {
    arr = lavrentev::getline(std::cin);
  } catch (const std::overflow_error &e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }

  if (arr.empty())
  {
    std::cout << "0\n";
    return 0;
  }

  lavrentev::LIter<std::pair<std::string, lavrentev::List<size_t>>> iterator = arr.begin();
  if ((*iterator).first == "")
  {
    return 0;
  }
  if ((*iterator).second.begin() == nullptr)
  {
    std::cout << (*iterator).first << "\n" << "0" << "\n";
    return 0;
  }
  iterator.printNames(std::cout);
  lavrentev::printTrans(arr);

  lavrentev::List<lavrentev::LIter<size_t>> iters = {};
  lavrentev::List<size_t> sums = {};

  while (iterator != arr.end())
  {
    lavrentev::LIter<size_t> it = {(*iterator).second.begin()};
    iters.insert(iters.end(), it);
    ++iterator;
  }

  lavrentev::LIter<lavrentev::LIter<size_t>> itersIt = iters.begin();

  while (true)
  {
    size_t sum = 0;
    iterator = arr.begin();
    while (itersIt != iters.end())
    {
      if (*itersIt != (*iterator).second.end())
      {
        size_t k = **itersIt;
        if (sum > std::numeric_limits<size_t>::max() - k)
        {
          std::cerr << "overflow" << "\n";
          return 1;
        }
        sum += k;
        ++(*itersIt);
      }
      ++itersIt;
      ++iterator;
    }
    if (sum == 0)
    {
      break;
    }
    sums.insert(sums.end(), sum);
    itersIt = iters.begin();
  }
  lavrentev::LIter<size_t> sumIt = sums.begin();
  sumIt.printList(std::cout);
}
