#include "List.hpp"
#include <iostream>
#include <string>

int main()
{
  saldaev::List< std::pair< std::string, saldaev::List< int > > > outer;

  std::string name = "";
  size_t num = 0;
  while (std::cin >> name) {
    saldaev::List< int > numList;
    while (std::cin >> num) {
      numList.newTail(num);
    }
    outer.newTail({name, std::move(numList)});

    if (!std::cin.eof()) {
      std::cin.clear();
    }
  }
}
