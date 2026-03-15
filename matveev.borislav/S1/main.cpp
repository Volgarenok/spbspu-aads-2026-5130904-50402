#include <iostream>
#include <string>
#include <utility>
#include "list.hpp"

int main()
{
  matveev::List< std::pair< std::string, matveev::List< int > > > sequences;
  std::string name;

  while (std::cin >> name)
  {
    matveev::List< int > numbers;
    int value;

    while (true)
    {
      if (std::cin.peek() == '\n' || std::cin.peek() == EOF)
      {
        break;
      }

      if (!(std::cin >> value))
      {
        break;
      }

      numbers.insertAfter(numbers.beforeBegin(), value);
    }

    std::pair< std::string, matveev::List< int > > seq(name, numbers);
    sequences.insertAfter(sequences.beforeBegin(), seq);
  }

  for (matveev::LIter< std::pair< std::string, matveev::List< int > > > it =
       sequences.begin(); it != sequences.end(); ++it)
  {
    std::cout << it->first << " ";
  }

  std::cout << "\n";
  return 0;
}
