#ifndef EVAL_HPP
#define EVAL_HPP
#include "queue.hpp"
#include "stack.hpp"
#include <string>

namespace bukreev
{
  using Expression = Queue< std::string >;
  using num_t = long long;

  Expression toPostfix(Expression infix);
  num_t evaluatePostfix(Expression postfix);
  num_t evaluateOperation(std::string op, num_t a, num_t b);
}

#endif
