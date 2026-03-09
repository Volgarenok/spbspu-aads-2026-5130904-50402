#include <cctype>
#include <iostream>
#include "List.hpp"

int main()
{
  size_t size;
  std::pair<std::string, lavrentev::List<int>*>* arr = lavrentev::getline(std::cin, size);
  for(size_t i = 0; i < size; ++i)
  {
    std::cout << arr[i].first << " ";
    lavrentev::LCIter<int> h(arr[i].second);
    h.printList();
  }
  for(size_t i = 0; i < size; ++i)
  {
    lavrentev::LIter<int>(arr[i].second).clear();
  }
  delete[] arr;
}

