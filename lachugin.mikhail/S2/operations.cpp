#include "operations.hpp"
#include <iostream>
#include <climits>

namespace lachugin
{
  int sum(int& a, int& b)
  {
    if ((a > INT_MAX - b) || (a < INT_MIN + b))
    {
      throw std::invalid_argument ("Bad numbers for sum");
    }
    return a + b;
  }

  int subtraction(int &a, int &b)
  {
    if (a < b + INT_MIN)
    {
      throw std::invalid_argument ("Bad numbers for subtraction");
    }
    return a - b;
  }

  int multiply(int& a, int& b)
  {
    if (a > 0 && b > 0 && a > INT_MAX / b)
    {
      throw std::invalid_argument("overflow");
    }
    if (a > 0 && b < 0 && b < INT_MIN / a)
    {
      throw std::invalid_argument("overflow");
    }
    if (a < 0 && b > 0 && a < INT_MIN / b)
    {
      throw std::invalid_argument("overflow");
    }
    if (a < 0 && b < 0 && a < INT_MAX / b)
    {
      throw std::invalid_argument("overflow");
    }

    return a * b;
  }

  int divide(int &a, int &b)
  {
    if (b == 0)
    {
      throw std::invalid_argument ("Bad numbers for divide");
    }
    return a / b;
  }

  int mod(int &a, int &b)
  {
    if (b == 0)
    {
      throw std::invalid_argument ("Bad numbers for mod");
    }
    return a % b;
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

  int operations(const std::string& op, int a, int b)
  {
    int res = 0;

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

