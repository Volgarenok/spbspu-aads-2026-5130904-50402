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

	bool isOpt(const std::string & candidate);
	bool getPriority(const std::string & op1, const std::string & op2);
	Queue< std::string > convert(Queue< std::string > inf);
	long long calc(const std::string & op, long long a, long long b);
	long long calcExpr(Queue< std::string > postfix);
	long long gcd(long long a, long long b);
}

#endif
