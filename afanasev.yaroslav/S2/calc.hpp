#ifndef CALC_HPP
#define CALC_HPP

#include <string>
#include <cstddef>
#include <limits>
#include <stdexcept>
#include "queue.hpp"
#include "stack.hpp"

namespace afanasev
{
	const std::string opts[] = { "+", "-", "*", "/", "%", "gcd" };
	const size_t opt_size = sizeof(opts) / sizeof(opts[0]);

	bool isOpt(const std::string & elem);
	bool getPriority(const std::string & op1, const std::string & op2);
	Queue< std::string > convert(Queue< std::string > inf);
	long long calc(const std::string & op, long long a, long long b);
	long long calcExpr(Queue< std::string > postfix);
	long long gcd(long long a, long long b);
}

bool afanasev::isOpt(const std::string & elem)
{
	for (size_t i = 0; i < opt_size; ++i)
	{
    if (elem == opts[i])
		{
			return true;
		}
  }
	return false;
}

bool afanasev::getPriority(const std::string & op1, const std::string & op2)
{
  int pr1 = 0, pr2 = 0;

  if (op1 == "gcd")
	{
		pr1 = 3
	}
  else if (op1 == "*" || op1 == "/" || op1 == "%")
	{
		pr1 = 2
	}
  else if (op1 == "+" || op1 == "-")
	{
		pr1 = 1
	}

  if (op2 == "gcd")
	{
		pr2 = 3
	}
  else if (op2 == "*" || op2 == "/" || op2 == "%")
	{
		pr2 = 2
	}
  else if (op2 == "+" || op2 == "-")
	{
		pr2 = 1
	}

  return pr1 >= pr2;
}

#endif
