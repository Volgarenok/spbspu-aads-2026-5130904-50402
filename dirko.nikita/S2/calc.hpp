#ifndef CALC_HPP
#define CALC_HPP
#include <string>
#include "queue.hpp"
namespace dirko
{
  const std::string opts[]{"+", "-", "*", "/", "%", "lcm"};
  Queue< std::string > convert(Queue< std::string > inf);
  long long calcExpr(Queue< std::string > expretion);
}
#endif
