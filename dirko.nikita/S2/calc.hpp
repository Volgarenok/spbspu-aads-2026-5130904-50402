#ifndef CALC_HPP
#define CALC_HPP
#include <string>
#include "queue.hpp"
namespace dirko
{
  const std::string opts[]{"+", "-", "*", "/", "%", "lcm"};
  Queue< std::string > convert(Queue< std::string > inf);
  long long calcExpr(Queue< std::string > expretion);
  bool getPriority(const std::string &op1, const std::string &op2);
  bool isOpt(std::string candidate);
  long long calc(const std::string &opt, long long a, long long b);
  long long lcm(long long a, long long b);
  long long sum(long long a, long long b);
  long long substract(long long a, long long b);
  long long multiply(long long a, long long b);
  long long divide(long long a, long long b);
  long long mod(long long a, long long b);
}
#endif
