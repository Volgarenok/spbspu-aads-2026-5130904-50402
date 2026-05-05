#ifndef MATVEEV_PARSER_HPP
#define MATVEEV_PARSER_HPP

#include "stack.hpp"
#include "queue.hpp"

#include <string>
#include <cctype>
#include <stdexcept>
#include <limits>
#include <cmath>

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
  return op == "+" || op == "-" || op == "*" ||
         op == "/" || op == "%" || op == "^";
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

bool willAddOverflow(long a, long b)
{
  if (b > 0 && a > std::numeric_limits<long>::max() - b)
  {
    return true;
  }

  if (b < 0 && a < std::numeric_limits<long>::min() - b)
  {
    return true;
  }

  return false;
}

bool willMulOverflow(long a, long b)
{
  if (a == 0 || b == 0)
  {
    return false;
  }

  if (a == -1 && b == std::numeric_limits<long>::min())
  {
    return true;
  }

  if (b == -1 && a == std::numeric_limits<long>::min())
  {
    return true;
  }

  if (a > std::numeric_limits<long>::max() / b)
  {
    return true;
  }

  if (a < std::numeric_limits<long>::min() / b)
  {
    return true;
  }

  return false;
}

long mod(long a, long b)
{
  long r = a % b;

  if (r < 0)
  {
    r += std::abs(b);
  }

  return r;
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

long evaluatePostfix(Queue<std::string> postfix)
{
  Stack<long> values;

  while (!postfix.empty())
  {
    std::string token = postfix.drop();

    if (isNumber(token))
    {
      values.push(std::stol(token));
    }
    else
    {
      if (values.size() < 2)
      {
        throw std::runtime_error("invalid expression");
      }

      long b = values.drop();
      long a = values.drop();

      if (token == "+")
      {
        if (willAddOverflow(a, b))
        {
          throw std::runtime_error("overflow");
        }

        values.push(a + b);
      }
      else if (token == "-")
      {
        if (willAddOverflow(a, -b))
        {
          throw std::runtime_error("overflow");
        }

        values.push(a - b);
      }
      else if (token == "*")
      {
        if (willMulOverflow(a, b))
        {
          throw std::runtime_error("overflow");
        }

        values.push(a * b);
      }
      else if (token == "/")
      {
        if (b == 0)
        {
          throw std::runtime_error("division by zero");
        }

        values.push(a / b);
      }
      else if (token == "%")
      {
        if (b == 0)
        {
          throw std::runtime_error("division by zero");
        }

        values.push(mod(a, b));
      }
      else if (token == "^")
      {
        long result = 1;

        for (long i = 0; i < b; ++i)
        {
          if (willMulOverflow(result, a))
          {
            throw std::runtime_error("overflow");
          }

          result *= a;
        }

        values.push(result);
      }
      else if (token == "gcd")
      {
        values.push(gcd(a, b));
      }
    }
  }

  if (values.size() != 1)
  {
    throw std::runtime_error("invalid expression");
  }

  return values.drop();
}

}

#endif
