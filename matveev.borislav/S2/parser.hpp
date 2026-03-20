#ifndef MATVEEV_PARSER_HPP
#define MATVEEV_PARSER_HPP

namespace matveev
{

long gcd(long a, long b)
{
  while (b != 0)
  {
    long t = b;
    b = a % b;
    a = t;
  }

  return a;
}

}

#endif
