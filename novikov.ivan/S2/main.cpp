#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept>
#include "stack.hpp"
#include "queue.hpp"

namespace novikov
{
  Stack< std::string > input(std::istream& in);
  Queue< std::string > split(const std::string& line);
  long long eval(std::string line);
  int getPriority(const std::string& op);
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
      return 3;
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
      return 3;
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
    catch (const std::invalid_argument&)
    {
      std::cerr << "Invalid arguments\n";
      return 1;
    }
    catch (const std::out_of_range&)
    {
      std::cerr << "Out of range\n";
      return 2;
    }
    catch (...)
    {
      std::cerr << "Unknown error\n";
      return 4;
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
    res.push(exp);
  }
  return res;
}

long long novikov::eval(std::string line)
{
  Queue< std::string > infix = split(line);
  Queue< std::string > postfix = infixToPostfix(infix);
  Stack< long long > results;
  results.push(0);
  long long operand1 = 0, operand2 = 0;
  int status = 0;

  while (!postfix.empty())
  {
    std::string val = postfix.front();
    if (val == "+")
    {
      if (status == 2)
      {
        results.push(operand1 + operand2);
      }
      else if (status == 1)
      {
        results.top() += operand1;
      }
      else if (status == 0)
      {
        long long op1 = results.top();
        results.pop();
        long long op2 = results.top();
        results.pop();
        results.top() = op1 + op2;
      }
      status = 0;
    }
    else if (val == "-")
    {
      if (status == 2)
      {
        results.push(operand1 - operand2);
      }
      else if (status == 1)
      {
        results.top() -= operand1;
      }
      else if (status == 0)
      {
        long long op1 = results.top();
        results.pop();
        long long op2 = results.top();
        results.pop();
        results.top() = op1 - op2;
      }
      status = 0;
    }
    else if (val == "*")
    {
      if (status == 2)
      {
        results.push(operand1 * operand2);
      }
      else if (status == 1)
      {
        results.top() *= operand1;
      }
      else if (status == 0)
      {
        long long op1 = results.top();
        results.pop();
        long long op2 = results.top();
        results.pop();
        results.top() = op1 * op2;
      }
      status = 0;
    }
    else if (val == "/")
    {
      if (status == 2)
      {
        results.push(operand1 / operand2);
      }
      else if (status == 1)
      {
        results.top() /= operand1;
      }
      else if (status == 0)
      {
        long long op1 = results.top();
        results.pop();
        long long op2 = results.top();
        results.pop();
        results.top() = op1 / op2;
      }
      status = 0;
    }
    else if (val == "%")
    {
      if (status == 2)
      {
        results.push(operand1 % operand2);
      }
      else if (status == 1)
      {
        results.top() %= operand1;
      }
      else if (status == 0)
      {
        long long op1 = results.top();
        results.pop();
        long long op2 = results.top();
        results.pop();
        results.top() = op1 % op2;
      }
      status = 0;
    }
    else if (val == "|")
    {
      if (status == 2)
      {
        results.push(operand1 | operand2);
      }
      else if (status == 1)
      {
        results.top() |= operand1;
      }
      else if (status == 0)
      {
        long long op1 = results.top();
        results.pop();
        long long op2 = results.top();
        results.pop();
        results.top() = op1 | op2;
      }
      status = 0;
    }
    else
    {
      if (status == 0)
      {
        operand1 = std::stoll(val);
        status = 1;
      }
      else if (status == 1)
      {
        operand2 = std::stoll(val);
        status = 2;
      }
    }
    postfix.pop();
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

bool novikov::isOperation(const std::string& s)
{
  bool res = s == "|";
  res = res || s == "*" || s == "/" || s == "%";
  res = res || s == "+" || s == "-";
  return res;
}

novikov::Queue< std::string > infixToPostfix(novikov::Queue< std::string > infix)
{
  novikov::Queue< std::string > postfix;
  novikov::Stack< std::string > stack;
  while (!postfix.empty())
  {
    std::string val = infix.front();
    if (val == "(")
    {
      stack.push(val);
    }
    else if (val == ")")
    {
      std::string op = stack.top();
      while (novikov::isOperation(op))
      {
        postfix.push(op);
        stack.pop();
        op = stack.top();
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
  return res;
}
