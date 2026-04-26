#ifndef OPERATIONS_HPP
#define OPERATIONS_HPP
#include <iostream>
namespace lachugin
{
  long long sum(long long& a, long long& b);
  long long subtraction(long long& a, long long& b);
  long long multiply(long long& a, long long& b);
  long long divide(long long& a, long long& b);
  long long mod(long long& a, long long& b);
  bool isPriority(const std::string& o1, const std::string& o2);
  long long operations(const std::string& o, long long a, long long b);

}
#endif

