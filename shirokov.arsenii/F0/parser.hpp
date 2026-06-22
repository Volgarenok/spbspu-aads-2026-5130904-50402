#ifndef PARSER_HPP
#define PARSER_HPP
#include <HashTable.hpp>
#include <Vector.hpp>
#include <hasher.hpp>
#include <string>
#include "lexer.hpp"
#include "matrix.hpp"

namespace shirokov
{
  enum class ValueType
  {
    NUMBER,
    MATRIX
  };

  struct RuntimeValue
  {
    ValueType type = ValueType::NUMBER;
    int num = 0;
    Matrix mat;

    RuntimeValue() = default;
    RuntimeValue(int);
    RuntimeValue(const Matrix&);
  };

  struct StringEqual
  {
    bool operator()(const std::string&, const std::string&) const;
  };

  using SymbolTable = HashTable< std::string, RuntimeValue, SHA1< std::string >, StringEqual >;
  using BuiltinFunction = RuntimeValue (*)(const Vector< RuntimeValue >&);
  using CommandTable = HashTable< std::string, BuiltinFunction, SHA1< std::string >, StringEqual >;

  class Parser
  {
  public:
    Parser(SymbolTable&, CommandTable&);
    void parseAndExecute(const Vector< Token >&);

  private:
    SymbolTable& symbols_;
    CommandTable& cmds_;
    size_t pos_;
    Vector< Token > tokens_;

    const Token& peek() const;
    const Token& advance();
    bool isAtEnd() const;
    bool match(TokenType);
    void consume(TokenType, const std::string&);

    void statement();
    RuntimeValue parseMatrix();
    RuntimeValue evaluatePrimary();
    RuntimeValue evaluateFactor();
    RuntimeValue evaluateTerm();
    RuntimeValue evaluateExpression();
  };
}

#endif
