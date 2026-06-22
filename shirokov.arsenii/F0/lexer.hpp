#ifndef LEXER_HPP
#define LEXER_HPP
#include <Vector.hpp>
#include <string>

namespace shirokov
{
  enum class TokenType
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
    POW,
    BAR,
    APOSTROPHE
  };

  struct Token
  {
    TokenType type;
    std::string value;
  };

  Vector< Token > tokenize(const std::string&);
}

#endif
