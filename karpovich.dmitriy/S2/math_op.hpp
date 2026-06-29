#ifndef MATH_OP_HPP
#define MATH_OP_HPP
#include <string>
#include "queue.hpp"

namespace karpovich
{
  Queue< std::string > convertToPostfix(Queue< std::string > infix);
  long long calculatePostfix(Queue< std::string > postfix);
}
#endif
