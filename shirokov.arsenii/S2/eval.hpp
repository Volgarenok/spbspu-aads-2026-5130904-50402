#ifndef EVAL_HPP
#define EVAL_HPP
#include <string>
#include "queue.hpp"

namespace
{
  std::string infixToPostfix(const std::string& infix);
  shirokov::Queue< std::string > split(const std::string& line);
}

namespace shirokov
{
  long long eval(std::string line);
}

#endif
