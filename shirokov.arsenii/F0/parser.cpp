#include "parser.hpp"
#include <iostream>
#include <stdexcept>

namespace
{
  bool isReservedName(const std::string& s)
  {
    return (s == "print" || s == "newline" || s == "zeros" || s == "ones" || s == "eye" || s == "fill" || s == "det"
            || s == "tr" || s == "transpose" || s == "concatH" || s == "concatV" || s == "concatDiag" || s == "addRow"
            || s == "addCol" || s == "delRow" || s == "delCol" || s == "resize" || s == "input");
  }

  int asNumber(const shirokov::RuntimeValue& v, const std::string& ctx)
  {
    if (v.type != shirokov::ValueType::NUMBER)
    {
      throw std::runtime_error(ctx);
    }
    return v.num;
  }

  const shirokov::Matrix& asMatrix(const shirokov::RuntimeValue& v, const std::string& ctx)
  {
    if (v.type != shirokov::ValueType::MATRIX)
    {
      throw std::runtime_error(ctx);
    }
    return v.mat;
  }

  shirokov::Vector< int > flattenOneRow(const shirokov::RuntimeValue& v, const std::string& ctx)
  {
    const shirokov::Matrix& m = asMatrix(v, ctx);
    shirokov::Vector< int > result;
    if (m.empty())
    {
      return result;
    }
    if (m.getRows() != 1)
    {
      throw std::runtime_error(ctx);
    }
    result.reserve(m.getCols());
    for (size_t j = 1; j <= m.getCols(); ++j)
    {
      result.pushBack(m.at(1, j));
    }
    return result;
  }
}

bool shirokov::StringEqual::operator()(const std::string& a, const std::string& b) const
{
  return a == b;
}
shirokov::RuntimeValue::RuntimeValue(int n):
  type(ValueType::NUMBER),
  num(n)
{}
shirokov::RuntimeValue::RuntimeValue(const Matrix& m):
  type(ValueType::MATRIX),
  num(0),
  mat(m)
{}

shirokov::Parser::Parser(SymbolTable& symbolTable, CommandTable& commandTable):
  symbols_(symbolTable),
  cmds_(commandTable),
  pos_(0),
  tokens_()
{}

const shirokov::Token& shirokov::Parser::peek() const
{
  if (isAtEnd())
  {
    return tokens_[tokens_.getSize() - 1];
  }
  return tokens_[pos_];
}

const shirokov::Token& shirokov::Parser::advance()
{
  if (!isAtEnd())
  {
    ++pos_;
  }
  return tokens_[pos_ - 1];
}

bool shirokov::Parser::isAtEnd() const
{
  return pos_ >= tokens_.getSize();
}
bool shirokov::Parser::match(TokenType type)
{
  if (isAtEnd() || peek().type != type)
  {
    return false;
  }
  advance();
  return true;
}

void shirokov::Parser::consume(TokenType type, const std::string& message)
{
  if (match(type))
  {
    return;
  }
  throw std::runtime_error(message);
}

namespace
{
  shirokov::Matrix matrixAdd(const shirokov::Matrix& a, const shirokov::Matrix& b)
  {
    if (a.getRows() != b.getRows() || a.getCols() != b.getCols())
    {
      throw std::invalid_argument("Matrix dimension mismatch");
    }
    shirokov::Matrix res = shirokov::zeros(a.getRows(), a.getCols());
    for (size_t i = 1; i <= a.getRows(); ++i)
      for (size_t j = 1; j <= a.getCols(); ++j)
        res.at(i, j) = a.at(i, j) + b.at(i, j);
    return res;
  }

  shirokov::Matrix matrixSub(const shirokov::Matrix& a, const shirokov::Matrix& b)
  {
    if (a.getRows() != b.getRows() || a.getCols() != b.getCols())
    {
      throw std::invalid_argument("Matrix dimension mismatch");
    }
    shirokov::Matrix res = shirokov::zeros(a.getRows(), a.getCols());
    for (size_t i = 1; i <= a.getRows(); ++i)
      for (size_t j = 1; j <= a.getCols(); ++j)
        res.at(i, j) = a.at(i, j) - b.at(i, j);
    return res;
  }
}

void shirokov::Parser::parseAndExecute(const Vector< Token >& tokens)
{
  tokens_ = tokens;
  pos_ = 0;
  if (!isAtEnd())
  {
    statement();
  }

  if (!isAtEnd())
  {
    throw std::runtime_error("Parser error: unexpected trailing tokens or unclosed parentheses");
  }
}

void shirokov::Parser::statement()
{
  if (match(TokenType::IDENTIFIER))
  {
    std::string name = tokens_[pos_ - 1].value;

    if (match(TokenType::ASSIGN))
    {
      if (isReservedName(name))
      {
        throw std::runtime_error("Parser error: cannot assign to reserved keyword '" + name + "'");
      }
      symbols_[name] = evaluateExpression();
      return;
    }

    if (match(TokenType::LBRACKET))
    {
      if (isReservedName(name))
      {
        throw std::runtime_error("Parser error: cannot modify elements of reserved keyword '" + name + "'");
      }

      RuntimeValue idxI = evaluateExpression();
      consume(TokenType::COMMA, "Expected ','");
      RuntimeValue idxJ = evaluateExpression();
      consume(TokenType::RBRACKET, "Expected ']'");
      consume(TokenType::ASSIGN, "Expected '='");
      RuntimeValue valK = evaluateExpression();

      if (!symbols_.contains(name))
      {
        throw std::runtime_error("Error: variable '" + name + "' does not exist");
      }
      if (symbols_.at(name).type != ValueType::MATRIX)
      {
        throw std::runtime_error("Not a matrix");
      }

      symbols_.at(name).mat.at(static_cast< size_t >(asNumber(idxI, "Matrix index must be an integer")),
                               static_cast< size_t >(asNumber(idxJ, "Matrix index must be an integer"))) =
          asNumber(valK, "Assigned value must be an integer");
      return;
    }

    if (isReservedName(name))
    {
      if (name == "addRow" || name == "addCol" || name == "delRow" || name == "delCol" || name == "resize"
          || name == "input")
      {
        consume(TokenType::LPAREN, "Expected '('");
        consume(TokenType::IDENTIFIER, "Expected variable name");
        std::string target = tokens_[pos_ - 1].value;

        if (isReservedName(target))
        {
          throw std::runtime_error("Parser error: cannot operate on reserved keyword '" + target + "'");
        }
        if (!symbols_.contains(target))
        {
          throw std::runtime_error("Error: variable '" + target + "' does not exist");
        }
        if (symbols_.at(target).type != ValueType::MATRIX)
        {
          throw std::runtime_error("Target must be a matrix");
        }

        if (name == "input")
        {
          consume(TokenType::RPAREN, "Expected ')'");
          if (!(std::cin >> symbols_.at(target).mat))
          {
            throw std::runtime_error("Input error: invalid matrix syntax");
          }
          return;
        }

        consume(TokenType::COMMA, "Expected ','");

        if (name == "addRow")
        {
          RuntimeValue rowVal = evaluateExpression();
          consume(TokenType::RPAREN, "Expected ')'");
          symbols_.at(target).mat.addRow(flattenOneRow(rowVal, "addRow expects a one-row matrix"));
          return;
        }

        if (name == "addCol")
        {
          RuntimeValue colVal = evaluateExpression();
          consume(TokenType::RPAREN, "Expected ')'");
          symbols_.at(target).mat.addCol(flattenOneRow(colVal, "addCol expects a one-row matrix"));
          return;
        }

        if (name == "delRow")
        {
          RuntimeValue idx = evaluateExpression();
          consume(TokenType::RPAREN, "Expected ')'");
          symbols_.at(target).mat.delRow(static_cast< size_t >(asNumber(idx, "delRow expects an integer")));
          return;
        }

        if (name == "delCol")
        {
          RuntimeValue idx = evaluateExpression();
          consume(TokenType::RPAREN, "Expected ')'");
          symbols_.at(target).mat.delCol(static_cast< size_t >(asNumber(idx, "delCol expects an integer")));
          return;
        }

        if (name == "resize")
        {
          RuntimeValue newN = evaluateExpression();
          consume(TokenType::COMMA, "Expected ','");
          RuntimeValue newM = evaluateExpression();

          int defK = 0;
          if (match(TokenType::COMMA))
          {
            RuntimeValue k = evaluateExpression();
            defK = asNumber(k, "resize default value must be an integer");
          }

          consume(TokenType::RPAREN, "Expected ')'");
          symbols_.at(target).mat.resize(static_cast< size_t >(asNumber(newN, "resize expects integers")),
                                         static_cast< size_t >(asNumber(newM, "resize expects integers")), defK);
          return;
        }
      }
    }

    --pos_;
  }

  evaluateExpression();
}

shirokov::RuntimeValue shirokov::Parser::parseMatrix()
{
  consume(TokenType::LBRACKET, "Expected '['");
  Vector< Vector< int > > mat;
  Vector< int > currentRow;
  size_t expectedCols = 0;
  bool closed = false;

  if (pos_ < tokens_.getSize() && tokens_[pos_].type == TokenType::RBRACKET)
  {
    ++pos_;
    return RuntimeValue(Matrix(mat));
  }

  while (pos_ < tokens_.getSize())
  {
    bool isNegative = false;
    if (tokens_[pos_].type == TokenType::MINUS)
    {
      isNegative = true;
      ++pos_;
    }

    if (pos_ >= tokens_.getSize() || tokens_[pos_].type != TokenType::NUMBER)
    {
      throw std::runtime_error("Matrix literal error: expected number");
    }

    int val = std::stoi(tokens_[pos_].value);
    ++pos_;
    if (isNegative)
    {
      val = -val;
    }
    currentRow.pushBack(val);

    if (pos_ < tokens_.getSize() && tokens_[pos_].type == TokenType::COMMA)
    {
      ++pos_;
    }
    else if (pos_ < tokens_.getSize() && tokens_[pos_].type == TokenType::SEMICOLON)
    {
      ++pos_;
      if (expectedCols == 0)
      {
        expectedCols = currentRow.getSize();
      }
      if (currentRow.getSize() != expectedCols)
      {
        throw std::runtime_error("Matrix literal error: rows must have equal length");
      }
      mat.pushBack(currentRow);
      currentRow = Vector< int >();
    }
    else if (pos_ < tokens_.getSize() && tokens_[pos_].type == TokenType::RBRACKET)
    {
      ++pos_;
      if (expectedCols == 0)
      {
        expectedCols = currentRow.getSize();
      }
      if (currentRow.getSize() != expectedCols)
      {
        throw std::runtime_error("Matrix literal error: rows must have equal length");
      }
      mat.pushBack(currentRow);
      closed = true;
      break;
    }
    else
    {
      throw std::runtime_error("Matrix literal error: expected separator or closing bracket");
    }
  }

  if (!closed)
  {
    throw std::runtime_error("Matrix literal error: missing closing bracket ']'");
  }
  return RuntimeValue(Matrix(mat));
}

shirokov::RuntimeValue shirokov::Parser::evaluatePrimary()
{
  if (pos_ >= tokens_.getSize())
  {
    throw std::runtime_error("Parser error: unexpected end of expression");
  }
  const Token& t = tokens_[pos_];

  if (t.type == TokenType::NUMBER)
  {
    ++pos_;
    return RuntimeValue(std::stoi(t.value));
  }
  if (t.type == TokenType::LBRACKET)
  {
    return parseMatrix();
  }
  if (t.type == TokenType::LPAREN)
  {
    ++pos_;
    RuntimeValue val = evaluateExpression();
    consume(TokenType::RPAREN, "Expected ')'");
    return val;
  }
  if (t.type == TokenType::IDENTIFIER)
  {
    std::string name = t.value;
    ++pos_;
    if (pos_ < tokens_.getSize() && tokens_[pos_].type == TokenType::LPAREN)
    {
      ++pos_;
      Vector< RuntimeValue > args;
      if (pos_ < tokens_.getSize() && tokens_[pos_].type != TokenType::RPAREN)
      {
        while (true)
        {
          args.pushBack(evaluateExpression());
          if (pos_ < tokens_.getSize() && tokens_[pos_].type == TokenType::COMMA)
          {
            ++pos_;
          }
          else
          {
            break;
          }
        }
      }
      consume(TokenType::RPAREN, "Expected ')'");
      if (!cmds_.contains(name))
      {
        throw std::runtime_error("Unknown function: " + name);
      }
      return cmds_.at(name)(args);
    }

    if (!symbols_.contains(name))
    {
      throw std::runtime_error("Error: variable '" + name + "' does not exist");
    }
    return symbols_.at(name);
  }
  throw std::runtime_error("Unexpected token");
}

shirokov::RuntimeValue shirokov::Parser::evaluateFactor()
{
  if (pos_ < tokens_.getSize() && tokens_[pos_].type == TokenType::BAR)
  {
    ++pos_;
    RuntimeValue inner = evaluateExpression();
    consume(TokenType::BAR, "Expected '|'");
    if (inner.type != ValueType::MATRIX)
    {
      throw std::runtime_error("Matrix error: determinant is only defined for matrices");
    }
    return RuntimeValue(inner.mat.det());
  }

  bool isNegative = false;
  if (pos_ < tokens_.getSize() && tokens_[pos_].type == TokenType::MINUS)
  {
    isNegative = true;
    ++pos_;
  }

  RuntimeValue val = evaluatePrimary();

  while (pos_ < tokens_.getSize() && tokens_[pos_].type == TokenType::APOSTROPHE)
  {
    if (val.type != ValueType::MATRIX)
    {
      throw std::runtime_error("Transpose is only defined for matrices");
    }
    ++pos_;
    val.mat = transpose(val.mat);
  }

  if (pos_ < tokens_.getSize() && tokens_[pos_].type == TokenType::POW)
  {
    ++pos_;
    RuntimeValue exp = evaluateFactor();
    if (exp.type != ValueType::NUMBER)
    {
      throw std::runtime_error("Runtime error: exponent must be an integer");
    }
    if (exp.num < 0)
    {
      throw std::runtime_error("Matrix error: exponent must be non-negative");
    }
    if (val.type != ValueType::MATRIX)
    {
      throw std::runtime_error("Matrix error: exponentiation is only defined for matrices");
    }
    val.mat = pow(val.mat, exp.num);
  }

  if (isNegative)
  {
    if (val.type == ValueType::NUMBER)
    {
      val.num = -val.num;
    }
    else
    {
      val.mat = val.mat * -1;
    }
  }

  return val;
}

shirokov::RuntimeValue shirokov::Parser::evaluateTerm()
{
  RuntimeValue val = evaluateFactor();

  while (pos_ < tokens_.getSize() && (tokens_[pos_].type == TokenType::MUL || tokens_[pos_].type == TokenType::DIV))
  {
    TokenType op = tokens_[pos_].type;
    ++pos_;
    RuntimeValue right = evaluateFactor();

    if (op == TokenType::DIV)
    {
      throw std::runtime_error("Division is not supported");
    }

    if (val.type == ValueType::NUMBER && right.type == ValueType::NUMBER)
    {
      val.num *= right.num;
    }
    else if (val.type == ValueType::MATRIX && right.type == ValueType::NUMBER)
    {
      val.mat = val.mat * right.num;
    }
    else if (val.type == ValueType::NUMBER && right.type == ValueType::MATRIX)
    {
      val = RuntimeValue(right.mat * val.num);
    }
    else if (val.type == ValueType::MATRIX && right.type == ValueType::MATRIX)
    {
      val.mat = val.mat * right.mat;
    }
    else
    {
      throw std::runtime_error("Type mismatch for multiplication");
    }
  }

  return val;
}

shirokov::RuntimeValue shirokov::Parser::evaluateExpression()
{
  RuntimeValue val = evaluateTerm();

  while (pos_ < tokens_.getSize() && (tokens_[pos_].type == TokenType::PLUS || tokens_[pos_].type == TokenType::MINUS))
  {
    TokenType op = tokens_[pos_].type;
    ++pos_;
    RuntimeValue right = evaluateTerm();

    if (val.type != right.type)
    {
      throw std::runtime_error("Type mismatch for arithmetic operation");
    }

    if (val.type == ValueType::NUMBER)
    {
      if (op == TokenType::PLUS)
      {
        val.num += right.num;
      }
      else
      {
        val.num -= right.num;
      }
    }
    else
    {
      if (op == TokenType::PLUS)
      {
        val.mat = matrixAdd(val.mat, right.mat);
      }
      else
      {
        val.mat = matrixSub(val.mat, right.mat);
      }
    }
  }

  return val;
}
