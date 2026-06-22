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

    if (c == '=')
    {
      if (i == 0 || source[i - 1] != ' ' || (i > 1 && source[i - 2] == ' '))
      {
        throw std::runtime_error("Lexer error: missing or extra space before '='");
      }
      if (i + 1 >= length || source[i + 1] != ' ' || (i + 2 < length && source[i + 2] == ' '))
      {
        throw std::runtime_error("Lexer error: missing or extra space after '='");
      }
      tokens.pushBack({TokenType::ASSIGN, "="});
      ++i;
      continue;
    }

    switch (c)
    {
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
    case '|':
      tokens.pushBack({TokenType::BAR, "|"});
      ++i;
      break;
    case '\'':
      tokens.pushBack({TokenType::APOSTROPHE, "'"});
      ++i;
      break;
    default:
      if (std::isspace(static_cast< unsigned char >(c)))
      {
        ++i;
      }
      else if (std::isdigit(static_cast< unsigned char >(c)))
      {
        std::string numStr;
        while (i < length && std::isdigit(static_cast< unsigned char >(source[i])))
        {
          numStr += source[i];
          ++i;
        }
        tokens.pushBack({TokenType::NUMBER, numStr});
      }
      else if (std::isalpha(static_cast< unsigned char >(c)) || c == '_')
      {
        std::string identStr;
        while (i < length && (std::isalnum(static_cast< unsigned char >(source[i])) || source[i] == '_'))
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
