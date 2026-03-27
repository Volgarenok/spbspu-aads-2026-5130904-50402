#include <iostream>
#include "list.hpp"

namespace bukreev
{
  using Sequence = std::pair< std::string, List< int > >;

  void input(std::istream& in, List< Sequence >& seqs);
  void output(std::ostream& out, const List< Sequence >& seqs);
}

int main()
{
  bukreev::List< bukreev::Sequence > sequences;

  bukreev::input(std::cin, sequences);
  bukreev::output(std::cout, sequences);
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

void bukreev::output(std::ostream& out, const List< Sequence >& seqs)
{
  LCIter< Sequence > it = seqs.cbegin();
  out << (*it).first;

  it = it.next();
  for (; it != seqs.cend(); it = it.next())
  {
    out << ' ' << (*it).first;
  }
  out << '\n';

  using ItPair = std::pair< LCIter< int >, LCIter< int > >;

  size_t size = seqs.size();
  ItPair* numIts = new ItPair[size];

  size_t i = 0;
  for (it = seqs.cbegin(); it != seqs.cend(); it = it.next(), i++)
  {
    numIts[i] = {(*it).second.cbegin(), (*it).second.cend()};
  }

  bukreev::List< int > sums;

  bool display = true;
  while (display)
  {
    display = false;
    int s = 0;
    for (i = 0; i < size; i++)
    {
      if (numIts[i].first != numIts[i].second)
      {
        display = true;

        int n = *numIts[i].first;
        out << n << ' ';
        s += n;

        numIts[i].first = numIts[i].first.next();
      }
    }

    if (display)
    {
      sums.pushBack(s);
      out << '\n';
    }
  }

  delete[] numIts;

  LCIter< int > sumit = sums.cbegin();
  if (sumit != sums.cend())
  {
    out << *sumit;
    sumit = sumit.next();
  }
  for (; sumit != sums.cend(); sumit = sumit.next())
  {
    out << ' ' << *sumit;
  }

  out << '\n';
}
