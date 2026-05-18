#ifndef EVAL_HPP
#define EVAL_HPP
#include "queue.hpp"
#include "stack.hpp"
#include <string>

namespace bukreev
{
  using Expression = Queue< std::string >;

  Expression toPostfix(Expression infix);
}

#endif
