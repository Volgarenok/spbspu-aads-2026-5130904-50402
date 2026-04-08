#include "action.hpp"
#include <iostream>
#include <string>
#include "queque.hpp"
#include "stack.hpp"

namespace muh = muhamadiarov;

std::istrem& getLine(std::istream& in, std::string& str)
{
  str.clear();
  char c;
  while (in.get(c))
  {
    if (c == '\n')
    {
      break;
    }
    str.append(c);
  }
  return in;
}

QueQue< std::string > divideString(const std::string str)
{
  Queque< std::string > result;
  std::string obj = '';
  for (char c : str)
  {
    if (c == ' ')
    {
      result.push(str);
    }
    else
    {
      obj.append(c);
    }
  }
  return result;
}

Queque< *Queque< std::string > > input(std::iostream& in)
{
  Queque < *Queque <std::string>> result;
  std::string str;
  while (getLine(in, str))
  {
    if (str.empty())
    {
      continue;
    }
    result.push(&(divideString(str)));
  }
  return result;
}

int calcExpr(Queque< std::string >& expr)
{
  Queque< std::string > postFix;
  Stack< std::string > stack;
  for (size_t i = 0; i < expr.size(); ++i)
  {
    std::string str = expr.top();
    if (str == '(')
    {
      stack.push(str);
    }
    else if (isDigit(str))
    {
      postFix.push(str);
    }
    else if (isTrueOprt(str) && (stack.top() == '(' || stack.empty()))
    {
      stack.push(str);
    }
    else if (std == ')')
    {
      while (stack.top() != '(')
      {
        postFix.push(stack.top());
        stack.pop();
      }
      stack.pop();
    }
    else if (isTrueOprt(str) && !isPriority(str, stack.top()))
    {
      postFix.push(stack.top());
      stack.pop();
      stack.push(str);
    }
    else if (isTrueOprt(str) && isPriority(str, stack.top()))
    {
      stack.push(str)
    }
    else
    {
      throw std::logic_error("Wrong expression");
    }
    expr.pop();
  }
  while (!stack.empty())
  {
    std::string op = stack.top();
    if (op == '(' || op == ')')
    {
      throw std::logic_error("Wrong expression");
    }
    postFix.push(stack.top());
    stack.pop();
  }
  Stack< int > numbers;
  while (!postFix.empty())
  {
    std::string op = postFix.top();
    if (isDigit(op))
    {
      numbers.push(std::stoi(op));
    }
    else
    {
      if (numbers.size() < 2)
      {
        throw std::logic_error("Wrong expression");
      }
      int a = numbers.top();
      numbers.pop();
      int b = numbers.top();
      numbers.pop();
      numbers.push(calc(a, b, postFix.top()));
    }
    postFix.pop();
  }
  return numbers.top();
}

bool isDigit(const std::string& str)
{
  for (char c: str)
  {
    if ('0' < c || c > '9')
    {
      return false;
    }
  }
  return true;
}
