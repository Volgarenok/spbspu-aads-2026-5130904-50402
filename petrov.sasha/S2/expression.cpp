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

  petrov::Queue< std::string > petrov::tokenize(const std::string& line)
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
}
#endif