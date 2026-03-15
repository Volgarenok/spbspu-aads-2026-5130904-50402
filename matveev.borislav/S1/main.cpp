#include <iostream>
#include <string>
#include <utility>
#include "list.hpp"

int main()
{
  matveev::List<std::pair<std::string,matveev::List<int>>> sequences;
  auto tail = sequences.beforeBegin();
  std::string name;

  while(std::cin>>name)
  {
    matveev::List<int> numbers;
    int value;
    auto numTail = numbers.beforeBegin();

    while(true)
    {
      int c = std::cin.peek();

      if(c=='\n'||c==EOF)
      {
        break;
      }

      if(!(std::cin>>value))
      {
        std::cin.clear();
        break;
      }

      numTail = numbers.insertAfter(numTail,value);
    }

    std::pair<std::string,matveev::List<int>> seq(name,numbers);
    tail = sequences.insertAfter(tail,seq);
  }

  for(matveev::LIter<std::pair<std::string,matveev::List<int>>> it = sequences.begin();
      it != sequences.end();
      ++it)
  {
    std::cout<<it->first<<" ";
  }

  std::cout<<"\n";
  return 0;
}
