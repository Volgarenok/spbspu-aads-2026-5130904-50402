#include "calculator.hpp"
#include <cctype>
#include <limits>
#include <stdexcept>
#include "queue.hpp"
#include "stack.hpp"

namespace pozdnyakov
{
  namespace
  {
    long long safeAdd(long long left, long long right)
    {
      if (right > 0 && left > std::numeric_limits< long long >::max() - right) {
        throw std::runtime_error("Overflow");
      }
      if (right < 0 && left < std::numeric_limits< long long >::min() - right) {
        throw std::runtime_error("Underflow");
      }
      return left + right;
    }

    long long safeSub(long long left, long long right)
    {
      if (right < 0 && left > std::numeric_limits< long long >::max() + right) {
        throw std::runtime_error("Overflow");
      }
      if (right > 0 && left < std::numeric_limits< long long >::min() + right) {
        throw std::runtime_error("Underflow");
      }
      return left - right;
    }

    long long safeMul(long long left, long long right)
    {
      if (left == 0 || right == 0) {
        return 0;
      }
      if (left > 0 && right > 0 && left > std::numeric_limits< long long >::max() / right) {
        throw std::runtime_error("Overflow");
      }
      if (left > 0 && right < 0 && right < std::numeric_limits< long long >::min() / left) {
        throw std::runtime_error("Underflow");
      }
      if (left < 0 && right > 0 && left < std::numeric_limits< long long >::min() / right) {
        throw std::runtime_error("Underflow");
      }
      if (left < 0 && right < 0 && left < std::numeric_limits< long long >::max() / right) {
        throw std::runtime_error("Overflow");
      }
      return left * right;
    }

    long long safeDiv(long long left, long long right)
    {
      if (right == 0) {
        throw std::runtime_error("Division by zero");
      }
      if (left == std::numeric_limits< long long >::min() && right == -1) {
        throw std::runtime_error("Overflow");
      }
      return left / right;
    }

    long long safeMod(long long left, long long right)
    {
      if (right == 0) {
        throw std::runtime_error("Modulo by zero");
      }
      if (left == std::numeric_limits< long long >::min() && right == -1) {
        throw std::runtime_error("Overflow");
      }
      return left % right;
    }
  }

  enum class TokenType { Number, Operator, LParen, RParen };

  struct Token
  {
    TokenType type;
    long long value;
    char op;
  };

  int getPrecedence(char op)
  {
    if (op == '&') {
      return 1;
    }
    if (op == '+' || op == '-') {
      return 2;
    }
    if (op == '*' || op == '/' || op == '%') {
      return 3;
    }
    return 0;
  }

  Queue< Token > tokenize(const std::string &expr)
  {
    Queue< Token > tokens;
    size_t i = 0;

    while (i < expr.length()) {
      if (std::isspace(expr[i])) {
        ++i;
        continue;
      }

      if (std::isdigit(expr[i])) {
        long long val = 0;
        while (i < expr.length() && std::isdigit(expr[i])) {
          int digit = expr[i] - '0';

          if (val > (std::numeric_limits< long long >::max() - digit) / 10) {
            throw std::runtime_error("Number too large");
          }

          val = val * 10 + digit;
          ++i;
        }
        tokens.push({TokenType::Number, val, '\0'});
      } else if (expr[i] == '(') {
        tokens.push({TokenType::LParen, 0, '\0'});
        ++i;
      } else if (expr[i] == ')') {
        tokens.push({TokenType::RParen, 0, '\0'});
        ++i;
      } else if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/' || expr[i] == '%'
                 || expr[i] == '&') {
        tokens.push({TokenType::Operator, 0, expr[i]});
        ++i;
      } else {
        throw std::runtime_error("Invalid character in expression");
      }
    }
    return tokens;
  }

  Queue< Token > infixToPostfix(Queue< Token > &infix)
  {
    Queue< Token > postfix;
    Stack< Token > operators;

    while (!infix.empty()) {
      Token token = infix.front();
      infix.pop();

      if (token.type == TokenType::Number) {
        postfix.push(token);
      } else if (token.type == TokenType::LParen) {
        operators.push(token);
      } else if (token.type == TokenType::RParen) {
        while (!operators.empty() && operators.top().type != TokenType::LParen) {
          postfix.push(operators.top());
          operators.pop();
        }
        if (operators.empty()) {
          throw std::runtime_error("Mismatched parentheses");
        }
        operators.pop();
      } else if (token.type == TokenType::Operator) {
        while (!operators.empty() && operators.top().type == TokenType::Operator
               && getPrecedence(operators.top().op) >= getPrecedence(token.op)) {
          postfix.push(operators.top());
          operators.pop();
        }
        operators.push(token);
      }
    }

    while (!operators.empty()) {
      if (operators.top().type == TokenType::LParen) {
        throw std::runtime_error("Mismatched parentheses");
      }
      postfix.push(operators.top());
      operators.pop();
    }

    return postfix;
  }

  long long evaluatePostfix(Queue< Token > &postfix)
  {
    if (postfix.empty()) {
      throw std::runtime_error("Empty expression");
    }

    Stack< long long > values;

    while (!postfix.empty()) {
      Token token = postfix.front();
      postfix.pop();

      if (token.type == TokenType::Number) {
        values.push(token.value);
      } else if (token.type == TokenType::Operator) {
        if (values.empty()) {
          throw std::runtime_error("Invalid expression");
        }
        long long right = values.top();
        values.pop();

        if (values.empty()) {
          throw std::runtime_error("Invalid expression");
        }
        long long left = values.top();
        values.pop();

        long long result = 0;
        switch (token.op) {
        case '+':
          result = safeAdd(left, right);
          break;
        case '-':
          result = safeSub(left, right);
          break;
        case '*':
          result = safeMul(left, right);
          break;
        case '/':
          result = safeDiv(left, right);
          break;
        case '%':
          result = safeMod(left, right);
          break;
        case '&':
          result = left & right;
          break;
        }
        values.push(result);
      }
    }

    if (values.empty()) {
      throw std::runtime_error("Invalid expression");
    }
    long long finalResult = values.top();
    values.pop();

    if (!values.empty()) {
      throw std::runtime_error("Invalid expression");
    }

    return finalResult;
  }

  long long evaluateExpression(const std::string &expression)
  {
    Queue< Token > infix = tokenize(expression);
    if (infix.empty()) {
      throw std::runtime_error("Empty expression");
    }
    Queue< Token > postfix = infixToPostfix(infix);
    return evaluatePostfix(postfix);
  }

}
