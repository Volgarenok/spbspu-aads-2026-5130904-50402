#ifndef MATVEEV_PARSER_HPP
#define MATVEEV_PARSER_HPP

#include "stack.hpp"
#include "queue.hpp"

#include <string>
#include <cctype>

namespace matveev
{

long gcd(long a, long b)
{
  while (b != 0)
  {
    long t = b;
    b = a % b;
    a = t;
  }

  return a;
}

bool isOperator(const std::string& op)
{
  return op == "+"
      || op == "-"
      || op == "*"
      || op == "/"
      || op == "%"
      || op == "^";
}

bool isGcd(const std::string& op)
{
  return op == "gcd";
}

int precedence(const std::string& op)
{
  if (op == "+" || op == "-")
  {
    return 1;
  }

  if (op == "*" || op == "/" || op == "%")
  {
    return 2;
  }

  if (op == "^")
  {
    return 3;
  }

  if (op == "gcd")
  {
    return 2;
  }

  return 0;
}

bool isNumber(const std::string& token)
{
  if (token.empty())
  {
    return false;
  }

  size_t i = 0;

  if (token[0] == '-' && token.size() > 1)
  {
    i = 1;
  }

  for (; i < token.size(); ++i)
  {
    if (!std::isdigit(token[i]))
    {
      return false;
    }
  }

  return true;
}

bool isLeftParen(const std::string& token)
{
  return token == "(";
}

bool isRightParen(const std::string& token)
{
  return token == ")";
}

Queue<std::string> toPostfix(Queue<std::string> input)
{
  Queue<std::string> output;
  Stack<std::string> operators;

  while (!input.empty())
  {
    std::string token = input.drop();

    if (isNumber(token))
    {
      output.push(token);
    }
    else if (isLeftParen(token))
    {
      operators.push(token);
    }
    else if (isRightParen(token))
    {
      while (!operators.empty() && !isLeftParen(operators.top()))
      {
        output.push(operators.drop());
      }

      if (!operators.empty())
      {
        operators.drop();
      }
    }
    else if (isOperator(token) || isGcd(token))
    {
      while (!operators.empty() &&
             (isOperator(operators.top()) || isGcd(operators.top())) &&
             precedence(operators.top()) >= precedence(token))
      {
        output.push(operators.drop());
      }

      operators.push(token);
    }
  }

  while (!operators.empty())
  {
    output.push(operators.drop());
  }

  return output;
}

}

#endif
