#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <List.hpp>
#include "Queue.hpp"
#include "Stack.hpp"
#include <stdexcept>
#include <climits>

namespace lavrentev
{
  Stack<long long> readFile(char *name);
  Stack<long long> getline();
  long long math(Queue<std::string> exp, long long &res);
  bool operCond(std::string buf);
  bool isNumber(std::string exp);
  long long gcd(long long a, long long b);
  long long count(long long a, long long b, long long &res, std::string operation);
  long long priority(const std::string &s);
}

inline long long lavrentev::math(lavrentev::Queue<std::string> exp, long long &res)
{
  Stack<long long> sRes{};
  Stack<std::string> op{};
  long long brackets = 0;
  while (!exp.empty())
  {
    std::string buf = exp.front();
    exp.drop();
    if (buf == "(")
    {
      op.push(buf);
      ++brackets;
    }
    else if (operCond(buf))
    {
      while (!op.empty() && op.top() != "(" && operCond(op.top()) &&
             priority(op.top()) >= priority(buf))
      {
        std::string operation = op.top();
        op.drop();
        long long oper2 = sRes.top();
        sRes.drop();
        long long oper1 = sRes.top();
        sRes.drop();
        long long bufAns = 0;
        if (!count(oper1, oper2, bufAns, operation))
        {
          sRes.push(bufAns);
        }
        else
        {
          return 1;
        }
      }
      op.push(buf);
    }
    else if (buf == ")")
    {
      op.drop();
      --brackets;
    }
    else
    {
      std::string operation;
      try
      {
        operation = op.top();
      }
      catch (...)
      {
        operation = "";
      }
      if (operCond(operation))
      {
        std::string expOper;
        try
        {
          expOper = exp.front();
        } catch (...)
        {
          expOper = "";
        }
        if (operCond(expOper) && priority(expOper) > priority(operation))
        {
          long long oper2;
          try
          {
            oper2 = std::stoll(buf);
          } catch (...)
          {
            std::cerr << "Invalid number" << "\n";
            return 2;
          }
          sRes.push(oper2);
        }
        else
        {
          op.drop();
          long long oper1 = sRes.top();
          sRes.drop();
          long long oper2;
          try
          {
            oper2 = std::stoll(buf);
          } catch (...)
          {
            std::cerr << "Invalid number" << "\n";
            return 2;
          }
          long long bufAns = 0;
          if (!count(oper1, oper2, bufAns, operation))
          {
            sRes.push(bufAns);
          }
          else
          {
            return 1;
          }
        }
      }
      else
      {
        try
        {
          sRes.push(std::stoll(buf));
        } catch (...)
        {
          std::cerr << "Invalid number" << "\n";
          return 2;
        }
      }
    }
  }
  while (!op.empty())
  {
    std::string operation = op.top();
    op.drop();
    if (operCond(operation))
    {
      long long oper2 = sRes.top();
      sRes.drop();
      long long oper1 = sRes.top();
      sRes.drop();
      long long bufAns = 0;
      if (!count(oper1, oper2, bufAns, operation))
      {
        sRes.push(bufAns);
      }
      else
      {
        return 1;
      }
    }
  }
  if (brackets)
  {
    return 2;
  }
  res = sRes.top();
  sRes.drop();
  return 0;
}

inline bool lavrentev::operCond(std::string buf)
{
  return buf == "+" || buf == "-" || buf == "*" || buf == "/" || buf == "gcd" || buf == "%";
}

inline long long lavrentev::gcd(long long a, long long b)
{
  while (a != 0 && b != 0)
  {
    if (a > b)
    {
      a = a % b;
    }
    else
    {
      b = b % a;
    }
  }
  return a + b;
}

inline long long lavrentev::count(long long a, long long b, long long &res, std::string operation)
{
  if (operation == "+")
  {
    if ((b > 0 && a > LLONG_MAX - b) || (b < 0 && a < LLONG_MIN - b))
    {
      throw std::out_of_range("Overflow error");
    }
    res = a + b;
    return 0;
  }
  else if (operation == "-")
  {
    if ((b < 0 && a > LLONG_MAX + b) || (b > 0 && a < LLONG_MIN + b))
    {
      throw std::out_of_range("Overflow error");
    }
    res = a - b;
    return 0;
  }
  else if (operation == "*")
  {
    bool c1 = a > 0 && b > 0 && a > LLONG_MAX / b;
    bool c2 = a < 0 && b < 0 && a < LLONG_MAX / b;
    bool c3 = a > 0 && b < 0 && b < LLONG_MIN / a;
    bool c4 = a < 0 && b > 0 && a < LLONG_MIN / b;
    if (c1 || c2 || c3 || c4)
    {
      throw std::out_of_range("Overflow error");
    }
    res = a * b;
    return 0;
  }
  else if (operation == "/")
  {
    if (b != 0)
    {
      if (a == LLONG_MIN && b == -1)
      {
        throw std::out_of_range("Overflow error");
      }
      res = a / b;
      return 0;
    }
    else
    {
      std::cerr << "Zero division";
      return 1;
    }
  }
  else if (operation == "gcd")
  {
    res = gcd(a, b);
    return 0;
  }
  else if (operation == "%")
  {
    if (b != 0)
    {
      if (a < 0 || b < 0)
      {
        res = a % b + b;
      }
      else
      {
        res = a % b;
      }
      return 0;
    }
    else
    {
      std::cerr << "Zero division";
      return 1;
    }
  }
  else
  {
    std::cerr << "Invalid operation";
    return 1;
  }
}

inline long long lavrentev::priority(const std::string &s)
{
  if (s == "+" || s == "-")
  {
    return 1;
  }
  else if (s == "*" || s == "/" || s == "%")
  {
    return 2;
  }
  else if (s == "gcd")
  {
    return 3;
  }
  return 0;
}

#endif
