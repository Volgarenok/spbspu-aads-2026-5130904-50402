#ifndef CALC_HPP
#define CALC_HPP
#include <string>
#include "queue.hpp"
namespace dirko
{
  const std::string opts[]{"+", "-", "*", "/", "%", "lcm"};
  constexpr size_t opt_size = 6;
  Queue< std::string > convert(Queue< std::string > inf);
  long long calc(Queue< std::string > expretion);
  bool getPriority(const std::string &op1, const std::string &op2);
  bool isOpt(std::string candidate);
}
#endif
