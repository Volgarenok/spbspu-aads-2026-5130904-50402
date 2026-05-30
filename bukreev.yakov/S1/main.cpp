#include <iostream>
#include <limits>
#include <list.hpp>

namespace bukreev
{
  using Sequence = std::pair< std::string, List< size_t > >;

  void input(std::istream& in, List< Sequence >& seqs);
  void output(std::ostream& out, const List< Sequence >& seqs);
}

int main()
{
  bukreev::List< bukreev::Sequence > sequences;

  try
  {
    bukreev::input(std::cin, sequences);
    bukreev::output(std::cout, sequences);
  }
  catch(const std::overflow_error& e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
}

void bukreev::input(std::istream& in, List< Sequence >& seqs)
{
  std::string name;

  while (in >> name)
  {
    List< size_t > list;

    size_t num;
    while (in >> num)
    {
      list.pushBack(num);
    }

    seqs.pushBack({name, list});

    if (in.eof())
    {
      break;
    }

    in.clear();
  }
}

void bukreev::output(std::ostream& out, const List< Sequence >& seqs)
{
  bool overflow = false;

  LCIter< Sequence > it = seqs.cbegin();
  if (it == seqs.cend())
  {
    out << "0\n";
    return;
  }
  out << (*it).first;
  it++;

  for (; it != seqs.cend(); it++)
  {
    out << ' ' << (*it).first;
  }
  out << '\n';

  using ItPair = std::pair< LCIter< size_t >, LCIter< size_t > >;

  size_t size = seqs.size();
  ItPair* numIts = new ItPair[size];

  size_t i = 0;
  for (it = seqs.cbegin(); it != seqs.cend(); it++, i++)
  {
    numIts[i] = {(*it).second.cbegin(), (*it).second.cend()};
  }

  bukreev::List< size_t > sums;

  bool display = true;
  while (display)
  {
    display = false;
    size_t s = 0;
    bool firstInLine = true;
    for (i = 0; i < size; i++)
    {
      if (numIts[i].first != numIts[i].second)
      {
        display = true;

        size_t n = *numIts[i].first;
        if (n > std::numeric_limits< size_t >::max() - s)
        {
          overflow = true;
        }
        s += n;

        if (firstInLine)
        {
          out << n;
          firstInLine = false;
        }
        else
        {
          out << ' ' << n;
        }

        numIts[i].first++;
      }
    }

    if (display)
    {
      sums.pushBack(s);
      out << '\n';
    }
  }

  delete[] numIts;

  if (overflow)
  {
    throw std::overflow_error("Integer overflow");
  }

  if (sums.size() == 0)
  {
    std::cout <<"0\n";
    return;
  }

  LCIter< size_t > sumit = sums.cbegin();
  if (sumit != sums.cend())
  {
    out << *sumit;
    sumit++;
  }
  for (; sumit != sums.cend(); sumit++)
  {
    out << ' ' << *sumit;
  }

  out << '\n';
}
