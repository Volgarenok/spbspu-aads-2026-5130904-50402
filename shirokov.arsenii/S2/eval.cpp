#include "eval.hpp"
#include <string>
#include "queue.hpp"
#include "stack.hpp"

namespace
{
  int getPriority(const std::string& op)
  {
    if (op == "<<")
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

  bool isOperation(const std::string& s)
  {
    bool res = s == "<<";
    res = res || s == "*" || s == "/" || s == "%";
    res = res || s == "+" || s == "-";
    return res;
  }

  shirokov::Queue< std::string > infixToPostfix(shirokov::Queue< std::string > infix)
  {
    shirokov::Queue< std::string > postfix;
    shirokov::Stack< std::string > stack;
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
        while (isOperation(op))
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
      else if (isOperation(val))
      {
        std::string op = stack.top();
        while (getPriority(op) <= getPriority(val))
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

  shirokov::Queue< std::string > split(const std::string& line, char delimiter = ' ')
  {
    shirokov::Queue< std::string > res;
    std::string curr;
    for (size_t i = 0; i < line.length(); ++i)
    {
      if (line[i] == delimiter)
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
}

long long shirokov::eval(std::string line)
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
    else if (val == "<<")
    {
      if (status == 2)
      {
        results.push(operand1 << operand2);
      }
      else if (status == 1)
      {
        results.top() <<= operand1;
      }
      else if (status == 0)
      {
        long long op1 = results.top();
        results.pop();
        long long op2 = results.top();
        results.pop();
        results.top() = op1 << op2;
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
