#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <string>
#include "queue.hpp"

namespace petrov {

  Queue< std::string > tokenize(const std::string& line);
  Queue< std::string > infixToPostfix(Queue< std::string >& tokens);
  long long evaluatePostfix(Queue< std::string >& postfix);

}

#endif

