#include "operations.hpp"
#include <iostream>
#include <climits>

namespace lachugin
{
  long long sum(long long& a, long long& b)
  {
    if ((a > LONG_LONG_MAX - b) || (a < LONG_LONG_MIN + b))
    {
      throw std::invalid_argument ("Bad numbers for sum");
    }
    return a + b;
  }

  long long subtraction(long long &a, long long &b)
  {
    if (a < b + LONG_LONG_MIN)
    {
      throw std::invalid_argument ("Bad numbers for subtraction");
    }
    return a - b;
  }

  long long multiply(long long& a, long long& b)
  {
    if (a > 0 && b > 0 && a > LONG_LONG_MAX / b)
    {
      throw std::invalid_argument("overflow");
    }
    if (a > 0 && b < 0 && b < LONG_LONG_MIN / a)
    {
      throw std::invalid_argument("overflow");
    }
    if (a < 0 && b > 0 && a < LONG_LONG_MIN / b)
    {
      throw std::invalid_argument("overflow");
    }
    if (a < 0 && b < 0 && a < LONG_LONG_MAX / b)
    {
      throw std::invalid_argument("overflow");
    }

    return a * b;
  }

  long long divide(long long &a, long long &b)
  {
    if (b == 0)
    {
      throw std::invalid_argument ("Bad numbers for divide");
    }
    return a / b;
  }

  long long mod(long long &a, long long &b)
  {
    if (b == 0)
    {
      throw std::invalid_argument ("Bad numbers for mod");
    }
    long long res = a % b;
    if (res < 0)
    {
      res += std::abs(b);
    }
    return res;
  }

  bool isPriority(const std::string& o1, const std::string& o2)
  {
    int a = 0, b = 0;

    if (o1 == "+" || o1 == "-")
    {
      a = 1;
    }
    else
    {
      a = 2;
    }

    if (o2 == "+" || o2 == "-")
    {
      b = 1;
    }
    else
    {
      b = 2;
    }

    return a >= b;
  }

  long long operations(const std::string& op, long long a, long long b)
  {
    long long res = 0;

    if (op == "+")
    {
      res = sum(a, b);
    }
    else if (op == "-")
    {
      res = subtraction(a, b);
    }
    else if (op == "*")
    {
      res = multiply(a, b);
    }
    else if (op == "/")
    {
      res = divide(a, b);
    }
    else if (op == "%")
    {
      res = mod(a, b);
    }
    return res;
  }

}

