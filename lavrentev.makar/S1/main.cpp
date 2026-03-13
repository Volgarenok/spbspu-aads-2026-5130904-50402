#include <cctype>
#include <iostream>
#include "List.hpp"

int main()
{
  lavrentev::List< std::pair< std::string, lavrentev::List< int > > > arr = {lavrentev::getline(std::cin)};
  lavrentev::LCIter<std::pair<std::string, lavrentev::List<int>>> it = arr.cbegin();
  //it.printList(std::cout);
  it.printNames(std::cout);
  while(it.hasNext())
  {
    
  }
}

