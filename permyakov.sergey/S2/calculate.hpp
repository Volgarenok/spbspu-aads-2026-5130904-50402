#ifndef CALCULATE_HPP
#define CALCULATE_HPP
#include <string>
#include <limits>
#include "queue.hpp"

const long long MAX = std::numeric_limits< long long >::max();
const long long MIN = std::numeric_limits< long long >::min();

namespace permyakov
{
  size_t getPriority(const std::string elm);

  Queue< std::string > convertToPostfix(Queue< std::string > exc);

  long long sum(const long long elm1, const long long elm2);
  long long sub(const long long elm1, const long long elm2);
  long long mult(const long long elm1, const long long elm2);
  long long div(const long long elm1, const long long elm2);
  long long mod(const long long elm1, const long long elm2);
  long long x_or(const long long elm1, const long long elm2);
  long long binCalculate(const std::string op, const long long elm1, const long long elm2);
  long long calculateExpression(Queue< std::string > exc);
}

#endif
