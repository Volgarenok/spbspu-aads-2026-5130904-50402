#include <iostream>
#include <limits>
#include <string>
#include "../common/queue.hpp"
#include "../common/stack.hpp"

namespace strelnikov
{
  extern const long long maxll;
  extern const long long minll;
  bool checkPriority(const std::string &, const std::string &);
  Queue< std::string > convertToPostfix(Queue< std::string >);
  Queue< Queue< std::string > > input(std::istream &);
  long long calcOps(const std::string &, long long, long long);
  long long calc(Queue< std::string >);
  long long notFunc(long long);
  bool isOp(const std::string &);
  void print(Queue< long long > &);
}
