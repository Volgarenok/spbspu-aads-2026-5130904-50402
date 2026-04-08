#include "tools.hpp"
#include "queue.hpp"
#include "stack.hpp"
#include <iostream>

namespace lachugin
{
  void input(std::istream& in, Queue<std::string>& q)
  {
    std::string token;

    while (in >> token)
    {
      q.push(token);
    }
  }

  bool isOperator(std::string& val)
  {
    return val == "+" || val == "-" || val == "*" || val == "/" || val == "%";
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
        while (operators.top() != "(" && !operators.empty())
        {
          postfix.push(operators.top());
          operators.pop();
        }
        operators.pop();
      }
      else if (isOperator(token))
      {
        if (!operators.empty() && isOperator(operators.top()))
        {
          postfix.push(operators.top());
          operators.pop();
        }
        operators.push(token);
      }
      else
      {
        postfix.push(token);
      }
    }
    while (!operators.empty())
    {
      if (operators.top() == "(") {
        throw std::invalid_argument ("Error: extra (");
      }
      postfix.push(operators.top());
      operators.pop();
    }
    return postfix;
  }


}