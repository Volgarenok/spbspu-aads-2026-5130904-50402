#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept>
#include <limits>
#include "stack.hpp"
#include "queue.hpp"

namespace novikov
{
  Stack< std::string > input(std::istream& in);
  Queue< std::string > split(const std::string& line);
  long long eval(std::string line);
  int getPriority(const std::string& op);
  bool isNumber(const std::string& line);
  bool isOperation(const std::string& s);
  Queue< std::string > infixToPostfix(novikov::Queue< std::string > infix);
}

int main(int argc, char** argv)
{
  novikov::Stack< std::string > expressions;
  if (argc == 1)
  {
    try
    {
      expressions = novikov::input(std::cin);
    }
    catch (const std::bad_alloc&)
    {
      std::cerr << "Bad allocation\n";
      return 2;
    }
  }
  else if (argc == 2)
  {
    try
    {
      std::ifstream in(argv[1]);
      expressions = novikov::input(in);
      in.close();
    }
    catch (const std::bad_alloc&)
    {
      std::cerr << "Bad allocation\n";
      return 2;
    }
  }
  else
  {
    std::cerr << "Invalid arguments\n";
    return 1;
  }

  while (!expressions.empty())
  {
    long long res;
    try
    {
      res = novikov::eval(expressions.top());
    }
    catch (...)
    {
      std::cerr << "Unknown error\n";
      return 3;
    }
    std::cout << res;
    expressions.pop();
    if (!expressions.empty())
    {
      std::cout << ' ';
    }
  }
  std::cout << '\n';
}

novikov::Stack< std::string > novikov::input(std::istream& in)
{
  Stack< std::string > res;
  std::string exp;
  while (std::getline(in, exp))
  {
    if (!exp.empty())
    {
      res.push(exp);
    }
  }
  return res;
}

long long novikov::eval(std::string line)
{
  Queue< std::string > infix = split(line);
  Queue< std::string > postfix = infixToPostfix(infix);
  Stack< long long > results;
  long long llmax = std::numeric_limits< long long >::max();
  long long llmin = std::numeric_limits< long long >::min();

  while (!postfix.empty())
  {
    std::string val = postfix.front();
    postfix.pop();
    if (isNumber(val))
    {
      results.push(std::stoll(val));
    }
    else
    {
      if (results.empty())
      {
        throw std::runtime_error("Not enough operands");
      }
      long long op2 = results.top();
      results.pop();
      if (results.empty())
      {
        throw std::runtime_error("Not enough operands");
      }
      long long op1 = results.top();
      results.pop();

      long long res = 0;
      if (val == "+")
      {
        if (op1 < 0 && op2 < 0 && op1 < llmin - op2)
        {
          throw std::underflow_error("Underflow");
        }
        else if (op1 > 0 && op2 > 0 && op1 > llmax - op2)
        {
          throw std::overflow_error("Overflow");
        }
      }
      else if (val == "-")
      {
        if (op1 > 0 && op2 < 0 && op1 > llmax + op2)
        {
          throw std::overflow_error("Overflow");
        }
        else if (op1 < 0 && op2 > 0 && op1 < llmin + op2)
        {
          throw std::underflow_error("Underflow");
        }
      }
      else if (val == "*")
      {
        if (((op1 > 0 && op2 > 0) || (op1 < 0 && op2 < 0)) && op1 > llmax / op2)
        {
          throw std::overflow_error("Overflow");
        }
        else if (((op1 > 0 && op2 < 0) || (op1 < 0 && op2 > 0)) && op1 < llmin / op2)
        {
          throw std::underflow_error("Underflow");
        }
      }
      else if (val == "/")
      {
        res = op1 / op2;
      }
      else if (val == "%")
      {
        res = op1 % op2;
        if (op1 < 0 || op2 < 0)
        {
          res += op2;
        }
      }
      else if (val == "|")
      {
        res = op1 | op2;
      }
      results.push(res);
    }
  }
  return results.top();
}

int novikov::getPriority(const std::string& op)
{
  if (op == "|")
  {
    return 3;
  }
  if (op == "*" || op == "/" || op == "%")
  {
    return 2;
  }
  if (op == "+" || op == "-")
  {
    return 1;
  }
  return 0;
}

bool isNumber(const std::string& line)
{
  for (size_t i = 0; i < line.length(); ++i)
  {
    if (!('0' <= line[i] && line[i] <= '9'))
    {
      return false;
    }
  }
  return true;
}

bool novikov::isOperation(const std::string& s)
{
  bool res = s == "|";
  res = res || s == "*" || s == "/" || s == "%";
  res = res || s == "+" || s == "-";
  return res;
}

novikov::Queue< std::string > novikov::infixToPostfix(novikov::Queue< std::string > infix)
{
  novikov::Queue< std::string > postfix;
  novikov::Stack< std::string > stack;
  while (!infix.empty())
  {
    std::string val = infix.front();
    if (val == "(")
    {
      stack.push(val);
    }
    else if (val == ")")
    {
      if (!stack.empty())
      {
        std::string op = stack.top();
        while (op != "(" && getPriority(op) >= getPriority(val))
        {
          postfix.push(op);
          stack.pop();
          if (stack.empty())
          {
            break;
          }
          op = stack.top();
        }
      }
      if (stack.top() == "(")
      {
        stack.pop();
      }
    }
    else if (novikov::isOperation(val))
    {
      std::string op = stack.top();
      while (novikov::getPriority(op) <= novikov::getPriority(val))
      {
        postfix.push(op);
        stack.pop();
        op = stack.top();
      }
      stack.push(val);
    }
    else
    {
      postfix.push(val);
    }
    infix.pop();
  }
  while (!stack.empty())
  {
    postfix.push(stack.top());
    stack.pop();
  }
  return postfix;
}

novikov::Queue< std::string > split(const std::string& line)
{
  novikov::Queue< std::string > res;
  std::string curr;
  for (size_t i = 0; i < line.length(); ++i)
  {
    if (line[i] == ' ')
    {
      res.push(curr);
      curr.clear();
    }
    else
    {
      curr += line[i];
    }
  }
  if (!curr.empty())
  {
    res.push(curr);
  }
  return res;
}
