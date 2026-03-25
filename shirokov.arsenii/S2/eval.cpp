#include "eval.hpp"
#include "stack.hpp"

long long shirokov::eval(std::string line)
{
  std::string postfix = infixToPostfix(line);
  Queue< std::string > exp = split(line);
  Stack< long long > results;
  results.push(0);
  long long operand1 = 0, operand2 = 0;
  int status = 0;

  while (!exp.empty())
  {
    std::string val = exp.front();
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
    exp.pop();
  }
  return results.top();
}
