#include <iostream>
#include "list.hpp"

namespace bukreev
{
  using Sequence = std::pair< std::string, List< int > >;

  void input(std::istream& in, List< Sequence >& seqs);
}

int main()
{
  bukreev::List< bukreev::Sequence > sequences;

  input(std::cin, sequences);
}

void bukreev::input(std::istream& in, List< Sequence >& seqs)
{
  std::string name;

  while (in >> name)
  {
    List< int > list;

    int num;
    while (in >> num)
    {
      list.pushBack(num);
    }

    seqs.pushBack({name, list});

    in.clear();
  }
}
