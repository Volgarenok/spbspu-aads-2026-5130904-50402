#include "tools.hpp"
#include "queue.hpp"
#include "stack.hpp"
#include "operations.hpp"
#include <iostream>


namespace lachugin
{
  void reedInput(std::istream& in, Queue< std::string >& q)
  {
    std::string token;

    while (in >> token)
    {
      q.push(token);
    }
  }

  bool isOperator(const std::string& val)
  {
    return val == "+" || val == "-" || val == "*" || val == "/" || val == "%" || val == "#";
  }

  Queue< std::string > infixToPostfix(Queue< std::string >& q)
  {
    Queue< std::string > postfix;
    Stack< std::string > operators;
    while (!q.empty())
    {
      std::string token = q.front();
      q.pop();

      if (token == "(")
      {
        operators.push(token);
      }
      else if (token == ")")
      {
        if (operators.empty())
        {
          throw std::invalid_argument ("Error with ) ");
        }
        while (!operators.empty() && operators.top() != "(")
        {
          postfix.push(operators.top());
          operators.pop();
        }
        operators.pop();
      }
      else if (isOperator(token))
      {
        if (token == "#")
        {
          operators.push(token);
          continue;
        }
        while (!operators.empty())
        {
          if (isOperator(operators.top()) && isPriority(operators.top(), token))
          {
            postfix.push(operators.top());
            operators.pop();
          }
          else
          {
            break;
          }
        }
        operators.push(token);
      }
      else
      {
        if (!operators.empty() && operators.top() == "#")
        {
          reversal(token);
          operators.pop();
        }
        postfix.push(token);
      }
    }
    while (!operators.empty())
    {
      if (operators.top() == "(")
      {
        throw std::invalid_argument ("Error: extra (");
      }
      postfix.push(operators.top());
      operators.pop();
    }
    return postfix;
  }

  void counting(Queue< std::string >& q, Stack< long long >& values)
  {
    while (!q.empty())
    {
      std::string token = q.front();
      q.pop();
      if (!isOperator(token))
      {
        long long value = std::stoll(token);
        values.push(value);
      }
      else
      {
        long long a, b, res;
        b = values.top();
        values.pop();
        a = values.top();
        values.pop();

        res = operations(token, a, b);
        values.push(res);
      }
    }
  }

}
