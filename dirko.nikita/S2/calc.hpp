#ifndef CALC_HPP
#define CALC_HPP
#include <string>
#include "queue.hpp"
namespace dirko
{
  Queue< std::string > convert(Queue< std::string > inf);
  long long calc(Queue< std::string > expretion);
  bool priority(const std::string &op1, const std::string &op2);
}
#endif
