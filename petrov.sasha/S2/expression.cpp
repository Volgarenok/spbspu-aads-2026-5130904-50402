#include "expression.hpp"
#include <stdexcept>
#include <string>
#include <stack.hpp>
#include <queue.hpp>

namespace petrov {
  bool isOperator(const std::string& token) {
    bool tok = token == '+' || token == '-' || token == '|' ||
    tok = token == '%' || token == '/' || token == '*';
    return tok;
  }

  int getPrecedence(const std::string& op) {
    if (op == '|') {
      return 1;
    }
    if (op == '+' || op == '-') {
      return 2;
    }
    if (op == '*' || op  == '/' || op == '%') {
      return 3;
    }
    return 0;
  }

  Queue< std::string > tokenize(const std::string& line)
  {
    Queue< std::string > tokens;
    std::size_t pos = 0;
    while (pos < line.size()) {
      const std::size_t spacePos = line.find(' ', pos);
      if (spacePos == std::string::npos) {
        const std::string token = line.substr(pos);
        if (!token.empty()) {
          tokens.push(token);
        }
        break;
      }
      if (spacePos > pos) {
        tokens.push(line.substr(pos, spacePos - pos));
      }
      pos = spacePos + 1;
    }
    return tokens;
  }
  Queue< std::string > infixToPostfix(Queue< std::string >& tokens)
  {
    Queue< std::string > output;
    Stack< std::string > ops;
    while (!tokens.empty()) {
      const std::string token = tokens.front();
      tokens.pop();
      if (token == "(") {
        ops.push(token);
      } else if (token == ")") {
        while (!ops.empty() && ops.top() != "(") {
          output.push(ops.top());
          ops.pop();
        }
        if (ops.empty()) {
          throw std::invalid_argument("Mismatched parentheses");
        }
        ops.pop();
      } else if (isOperator(token)) {
        const int prec = getPrecedence(token);
        while (!ops.empty() && ops.top() != "(" && getPrecedence(ops.top()) >= prec) {
          output.push(ops.top());
          ops.pop();
        }
        ops.push(token);
      } else {
        output.push(token);
      }
    }
    while (!ops.empty()) {
      if (ops.top() == "(" || ops.top() == ")") {
        throw std::invalid_argument("Mismatched parentheses");
      }
      output.push(ops.top());
      ops.pop();
    }
    return output;
  }
  long long evaluatePostfix(Queue< std::string >& postfix)
  {
    Stack< long long > operands;
    while (!postfix.empty()) {
      const std::string token = postfix.front();
      postfix.pop();
      if (isOperator(token)) {
        if (operands.empty()) {
          throw std::invalid_argument("Invalid expression");
        }
        const long long right = operands.top();
        operands.pop();
        if (operands.empty()) {
          throw std::invalid_argument("Invalid expression");
        }
        const long long left = operands.top();
        operands.pop();
        long long result = 0;
        if (token == "+") {
          result = left + right;
        } else if (token == "-") {
          result = left - right;
        } else if (token == "*") {
          result = left * right;
        } else if (token == "/") {
          if (right == 0) {
            throw std::invalid_argument("Division by zero");
          }
          result = left / right;
        } else if (token == "%") {
          if (right == 0) {
            throw std::invalid_argument("Division by zero");
          }
          result = left % right;
        } else if (token == "|") {
          result = left | right;
        }
        operands.push(result);
      } else {
        try {
          const long long value = std::stoll(token);
          operands.push(value);
        } catch (const std::exception&) {
          throw std::invalid_argument("Invalid operand");
        }
      }
    }
    if (operands.empty()) {
      throw std::invalid_argument("Empty expression");
    }
    const long long finalResult = operands.top();
    operands.pop();
    if (!operands.empty()) {
      throw std::invalid_argument("Invalid expression");
    }
    return finalResult;
  }
}
#endif

