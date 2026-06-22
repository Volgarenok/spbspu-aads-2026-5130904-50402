#include "lexer.hpp"
#include <cctype>
#include <stdexcept>

shirokov::Vector< shirokov::Token > shirokov::tokenize(const std::string& source)
{
  Vector< Token > tokens;
  size_t i = 0;
  size_t length = source.length();

  while (i < length)
  {
    char c = source[i];
    switch (c)
    {
    case '=':
      tokens.pushBack({TokenType::ASSIGN, "="});
      ++i;
      break;
    case '[':
      tokens.pushBack({TokenType::LBRACKET, "["});
      ++i;
      break;
    case ']':
      tokens.pushBack({TokenType::RBRACKET, "]"});
      ++i;
      break;
    case '(':
      tokens.pushBack({TokenType::LPAREN, "("});
      ++i;
      break;
    case ')':
      tokens.pushBack({TokenType::RPAREN, ")"});
      ++i;
      break;
    case ',':
      tokens.pushBack({TokenType::COMMA, ","});
      ++i;
      break;
    case ';':
      tokens.pushBack({TokenType::SEMICOLON, ";"});
      ++i;
      break;
    case '+':
      tokens.pushBack({TokenType::PLUS, "+"});
      ++i;
      break;
    case '-':
      tokens.pushBack({TokenType::MINUS, "-"});
      ++i;
      break;
    case '*':
      tokens.pushBack({TokenType::MUL, "*"});
      ++i;
      break;
    case '/':
      tokens.pushBack({TokenType::DIV, "/"});
      ++i;
      break;
    case '^':
      tokens.pushBack({TokenType::POW, "^"});
      ++i;
      break;
    default:
      if (std::isspace(c))
      {
        ++i;
      }
      else if (std::isdigit(c) || c == '.')
      {
        std::string numStr;
        bool hasDot = false;
        while (i < length && (std::isdigit(source[i]) || source[i] == '.'))
        {
          if (source[i] == '.')
          {
            if (hasDot)
            {
              throw std::runtime_error("Lexer error: invalid number format with multiple dots");
            }
            hasDot = true;
          }
          numStr += source[i];
          ++i;
        }
        tokens.pushBack({TokenType::NUMBER, numStr});
      }
      else if (std::isalpha(c) || c == '_')
      {
        std::string identStr;
        while (i < length && (std::isalnum(source[i]) || source[i] == '_'))
        {
          identStr += source[i];
          ++i;
        }
        tokens.pushBack({TokenType::IDENTIFIER, identStr});
      }
      else
      {
        throw std::runtime_error(std::string("Lexer error: unexpected character '") + c + "'");
      }
      break;
    }
  }

  return tokens;
}
