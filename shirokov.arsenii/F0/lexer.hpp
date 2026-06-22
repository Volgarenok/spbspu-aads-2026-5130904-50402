#ifndef LEXER_HPP
#define LEXER_HPP
#include <string>
#include "Vector.hpp"

namespace shirokov
{
  enum TokenType
  {
    IDENTIFIER,
    NUMBER,
    ASSIGN,
    LBRACKET,
    RBRACKET,
    LPAREN,
    RPAREN,
    COMMA,
    SEMICOLON,
    PLUS,
    MINUS,
    MUL,
    DIV,
    POW
  };

  struct Token
  {
    TokenType type;
    std::string value;
  };

  Vector< Token > tokenize(const std::string&);
}

#endif
