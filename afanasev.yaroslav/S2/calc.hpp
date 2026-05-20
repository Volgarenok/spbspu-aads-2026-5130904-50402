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
    pr1 = 3;
  }
  else if (op1 == "*" || op1 == "/" || op1 == "%")
  {
    pr1 = 2;
  }
  else if (op1 == "+" || op1 == "-")
  {
    pr1 = 1;
  }

  if (op2 == "gcd")
  {
    pr2 = 3;
  }
  else if (op2 == "*" || op2 == "/" || op2 == "%")
  {
    pr2 = 2;
  }
  else if (op2 == "+" || op2 == "-")
  {
    pr2 = 1;
  }

  return pr1 >= pr2;
}

afanasev::Queue< std::string > afanasev::convert(Queue< std::string > inf)
{
  Queue< std::string > out;
  Stack< std::string > tmp;

  while (!inf.empty())
  {
    std::string tk = inf.get();
    inf.pop();

    if (tk == "(")
    {
      tmp.push(tk);
    }
    else if (isOpt(tk))
    {
      while (!tmp.empty() && tmp.get() != "(" && getPriority(tmp.get(), tk))
      {
        out.push(tmp.get());
        tmp.pop();
      }

      tmp.push(tk);
    }
    else if (tk == ")")
    {
      while (!tmp.empty() && tmp.get() != "(")
      {
        out.push(tmp.get());
        tmp.pop();
      }

      if (!tmp.empty() && tmp.get() == "(")
      {
        tmp.pop();
      }
    }
    else
    {
      out.push(tk);
    }
  }

  while (!tmp.empty())
  {
    out.push(tmp.get());
    tmp.pop();
  }

  return out;
}

long long afanasev::calc(const std::string & op, long long a, long long b)
{
  if (op == "+")
  {
    if (a > 0 && b > 0 && a > std::numeric_limits< long long >::max() - b)
    {
      throw std::overflow_error("overflow in addition");
    }
    if (a < 0 && b < 0 && a < std::numeric_limits< long long >::min() - b)
    {
      throw std::underflow_error("underflow in addition");
    }
    return a + b;
  }

  if (op == "-")
  {
    if (b > 0 && a < std::numeric_limits< long long >::min() + b)
    {
      throw std::underflow_error("underflow in subtraction");
    }
    if (b < 0 && a > std::numeric_limits< long long >::max() + b)
    {
      throw std::overflow_error("overflow in subtraction");
    }
    return a - b;
  }

  if (op == "*")
  {
    if (!a || !b)
    {
      return 0;
    }
    if (a > 0 && b > 0 && a > std::numeric_limits< long long >::max() / b)
    {
      throw std::overflow_error("overflow in multiplication");
    }
    if (a > 0 && b < 0 && b < std::numeric_limits< long long >::min() / a)
    {
      throw std::overflow_error("overflow in multiplication");
    }
    if (a < 0 && b > 0 && a < std::numeric_limits< long long >::min() / b)
    {
      throw std::overflow_error("overflow in multiplication");
    }
    if (a < 0 && b < 0 && a / b > 0 && (a / b) > std::numeric_limits< long long >::max() / b)
    {
      throw std::overflow_error("overflow in multiplication");
    }
    return a * b;
  }

  if (op == "/")
  {
    if (!b)
    {
      throw std::logic_error("division by zero");
    }
    return a / b;
  }

  if (op == "%")
  {
    if (!b)
    {
      throw std::logic_error("modulo by zero");
    }
    return (a % b + b) % b;
  }

  if (op == "gcd")
  {
    return gcd(a, b);
  }
  throw std::runtime_error("unknown operator: " + op);
}

long long afanasev::calcExpr(Queue< std::string > postfix)
{
  Stack<long long> st;

  while (!postfix.empty())
  {
    std::string tk = postfix.get();
    postfix.pop();

    if (isOpt(tk))
    {
      if (st.empty())
      {
        throw std::runtime_error("not enough operands");
      }
      long long b = st.get();
      st.pop();

      if (st.empty())
      {
        throw std::runtime_error("not enough operands");
      }
      long long a = st.get();
      st.pop();
      st.push(calc(tk, a, b));
    }
    else
    {
      st.push(std::stoll(tk));
    }
  }

  if (st.empty())
  {
    throw std::runtime_error("empty expression");
  }
  return st.get();
}

long long afanasev::gcd(long long a, long long b)
{
  a = llabs(a);
  b = llabs(b);
  while (b)
  {
    long long t = b;
    b = a % b;
    a = t;
  }
  return a;
}

#endif
