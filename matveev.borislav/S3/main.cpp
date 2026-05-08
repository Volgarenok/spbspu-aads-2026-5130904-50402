#include "graphOutput.hpp"

#include <iostream>
#include <string>

int main()
{
  matveev::List< matveev::EdgeOutput > rows;

  matveev::addEdgeOutput(rows, "b", 20);
  matveev::addEdgeOutput(rows, "a", 30);
  matveev::addEdgeOutput(rows, "b", 10);

  for (matveev::LIter< matveev::EdgeOutput > it = rows.begin(); it != rows.end(); ++it)
  {
    std::cout << it->vertex;

    for (matveev::LIter< unsigned long long > weight = it->weights.begin(); weight != it->weights.end(); ++weight)
    {
      std::cout << ' ' << *weight;
    }

    std::cout << '\n';
  }

  return 0;
}
