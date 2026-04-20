#include "action.hpp"
#include <iostream>
#include <string>
#include "queque.hpp"
#include "stack.hpp"

namespace muh = muhamadiarov;

std::istream& muh::getLine(std::istream& in, std::string& str)
{
  str.clear();
  char c;
  while (in.get(c))
  {
    if (c == '\n')
    {
      break;
    }
    str += c;
  }
  return in;
}

muh::Queque<std::string> muh::divideString(const std::string str)
{
  Queque<std::string> result;
  std::string obj = "";
  for (char c : str)
  {
    if (c == ' ' && obj != "")
    {
      result.push(obj);
      obj = "";
    }
    else if (c != ' ')
    {
      obj += c;
    }
  }
  result.push(obj);
  return result;
}

muh::Queque< muh::Queque< std::string > > muh::input(std::istream& in)
{
  Queque< Queque < std::string > > result;
  std::string str;
  while (getLine(in, str))
  {
    if (str.empty())
    {
      continue;
    }
    result.push(divideString(str));
  }
  return result;
}

int muh::calcExpr(Queque< std::string >& expr)
{
  Queque< std::string > postFix;
  Stack< std::string > stack;
  size_t sizeExpr = expr.size();
  for (size_t i = 0; i < sizeExpr; ++i)
  {
    std::string str = expr.top();
    if (str == "(")
    {
      stack.push(str);
    }
    else if (isDigit(str))
    {
      postFix.push(str);
    }
    else if (isTrueOprt(str) && ((!stack.empty() && stack.top() == "(") || stack.empty()))
    {
      stack.push(str);
    }
    else if (str == ")")
    {
      while (!stack.empty() && stack.top() != "(")
      {
        postFix.push(stack.top());
        stack.pop();
      }
      stack.pop();
    }
    else if (!stack.empty() && isTrueOprt(str) && !isPriority(str, stack.top()))
    {
      postFix.push(stack.top());
      stack.pop();
      stack.push(str);
    }
    else if (!stack.empty() && isTrueOprt(str) && isPriority(str, stack.top()))
    {
      stack.push(str);
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
    if (op == "(" || op == ")")
    {
      throw std::logic_error("Wrong expression: '(' or ')' are placed incorrectly");
    }
    postFix.push(op);
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
        throw std::logic_error("Wrong expression: order of numbers is incorrect");
      }
      int a = numbers.top();
      numbers.pop();
      int b = numbers.top();
      numbers.pop();
      numbers.push(calc(b, a, postFix.top()));
    }
    postFix.pop();
  }
  return numbers.top();
}

bool muh::isDigit(const std::string str)
{
  size_t i = 0;
  for (char c: str)
  {
    if (str.size() > 1 && i == 0 && c == '0')
    {
      return false;
    }
    if (c < '0' || c > '9')
    {
      return false;
    }
    ++i;
  }
  return true;
}

bool muh::isTrueOprt(const std::string op)
{
  for (size_t i = 0; i < 6; ++i)
  {
    if (oprts[i].first == op)
    {
      return true;
    }
  }
  return false;
}

bool muh::isPriority(const std::string op1, const std::string op2)
{
  size_t ind1 = -1;
  size_t ind2 = -1;
  for (size_t i  = 0; i < 6; ++i)
  {
    if (oprts[i].first == op1)
    {
      ind1 = i;
    }
    else if (oprts[i].first == op2)
    {
      ind2 = i;
    }
  }
  return ind1 > ind2;
}

int muh::calc(int a, int b, const std::string op)
{
  for (size_t i = 0; i < 6; ++i)
  {
    if (oprts[i].first == op)
    {
      return oprts[i].second(a, b);
    }
  }
  throw std::logic_error("Not correct operation");
}

int muh::sum(int a, int b)
{
  return a + b;
}

int muh::div(int a, int b)
{
  return a / b;
}

int muh::sub(int a, int b)
{
  return a - b;
}

int muh::mult(int a, int b)
{
  return a * b;
}

int muh::mod(int a, int b)
{
  return a % b;
}

int muh::xcor(int a, int b)
{
  int result = 0;
  int multiplier = 1;
  while (a > 0 || b > 0){
    int bitA = a % 2;
    int bitB = b % 2;
    int xorBit = (bitA != bitB) ? 1 : 0;
    result += xorBit * multiplier;
    a /= 2;
    b /= 2;
    multiplier *= 2;
  }
  return result;
}
