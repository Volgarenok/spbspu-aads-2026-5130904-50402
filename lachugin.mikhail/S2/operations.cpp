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

  int multiply(int &a, int &b)
  {
    if ((a > INT_MAX / b) || (a < INT_MIN / b))
    {
      throw std::invalid_argument ("Bad numbers for multiply");
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


}
