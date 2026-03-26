#include "calculator.hpp"
#include "stack.hpp"
#include "queue.hpp"
#include <cctype>
#include <stdexcept>

namespace pozdnyakov
{
  enum class TokenType
  {
    Number,
    Operator,
    LParen,
    RParen
  };

  struct Token
  {
    TokenType type;
    long long value;
    char op;
  };

  int getPrecedence(char op)
  {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/' || op == '%') return 2;
    return 0;
  }

  Queue< Token > tokenize(const std::string& expr)
  {
    Queue< Token > tokens;
    std::size_t i = 0;

    while (i < expr.length()) {
      if (std::isspace(expr[i])) {
        ++i;
        continue;
      }

      if (std::isdigit(expr[i])) {
        long long val = 0;
        while (i < expr.length() && std::isdigit(expr[i])) {
          val = val * 10 + (expr[i] - '0');
          ++i;
        }
        tokens.push({ TokenType::Number, val, '\0' });
      }
      else if (expr[i] == '(') {
        tokens.push({ TokenType::LParen, 0, '\0' });
        ++i;
      }
      else if (expr[i] == ')') {
        tokens.push({ TokenType::RParen, 0, '\0' });
        ++i;
      }
      else if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/' || expr[i] == '%') {
        tokens.push({ TokenType::Operator, 0, expr[i] });
        ++i;
      }
      else {
        throw std::runtime_error("Invalid character in expression");
      }
    }
    return tokens;
  }

  Queue< Token > infixToPostfix(Queue< Token >& infix)
  {
    Queue< Token > postfix;
    Stack< Token > operators;

    while (!infix.empty()) {
      Token token = infix.front();
      infix.pop();

      if (token.type == TokenType::Number) {
        postfix.push(token);
      }
      else if (token.type == TokenType::LParen) {
        operators.push(token);
      }
      else if (token.type == TokenType::RParen) {
        while (!operators.empty() && operators.top().type != TokenType::LParen) {
          postfix.push(operators.top());
          operators.pop();
        }
        if (operators.empty()) {
          throw std::runtime_error("Mismatched parentheses");
        }
        operators.pop();
      }
      else if (token.type == TokenType::Operator) {
        while (!operators.empty() && operators.top().type == TokenType::Operator &&
          getPrecedence(operators.top().op) >= getPrecedence(token.op)) {
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

  long long evaluatePostfix(Queue< Token >& postfix)
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
      }
      else if (token.type == TokenType::Operator) {
        if (values.empty()) throw std::runtime_error("Invalid expression");
        long long right = values.top();
        values.pop();

        if (values.empty()) throw std::runtime_error("Invalid expression");
        long long left = values.top();
        values.pop();

        long long result = 0;
        switch (token.op) {
        case '+': result = left + right; break;
        case '-': result = left - right; break;
        case '*': result = left * right; break;
        case '/':
          if (right == 0) throw std::runtime_error("Division by zero");
          result = left / right;
          break;
        case '%':
          if (right == 0) throw std::runtime_error("Modulo by zero");
          result = left % right;
          break;
        }
        values.push(result);
      }
    }

    if (values.empty()) throw std::runtime_error("Invalid expression");
    long long finalResult = values.top();
    values.pop();

    if (!values.empty()) throw std::runtime_error("Invalid expression");

    return finalResult;
  }

  long long evaluateExpression(const std::string& expression)
  {
    Queue< Token > infix = tokenize(expression);
    if (infix.empty()) {
      throw std::runtime_error("Empty expression");
    }
    Queue< Token > postfix = infixToPostfix(infix);
    return evaluatePostfix(postfix);
  }

}
