#include "commands.hpp"
#include <iostream>
#include <stdexcept>
#include <string>
#include "matrix.hpp"
#include "parser.hpp"

namespace
{
  using shirokov::RuntimeValue;
  using shirokov::ValueType;

  void expectCount(const shirokov::Vector< RuntimeValue >& args, size_t expected, const char* fn)
  {
    if (args.getSize() != expected)
    {
      throw std::runtime_error(std::string(fn) + " expects " + std::to_string(expected) + " argument(s)");
    }
  }

  int asNumber(const RuntimeValue& v, const char* fn)
  {
    if (v.type != ValueType::NUMBER)
    {
      throw std::runtime_error(std::string(fn) + " expects integer arguments");
    }
    return v.num;
  }

  const shirokov::Matrix& asMatrix(const RuntimeValue& v, const char* fn)
  {
    if (v.type != ValueType::MATRIX)
    {
      throw std::runtime_error(std::string(fn) + " expects matrix arguments");
    }
    return v.mat;
  }
}

shirokov::RuntimeValue shirokov::builtinPrint(const Vector< RuntimeValue >& args)
{
  expectCount(args, 1, "print");

  if (args[0].type == ValueType::NUMBER)
  {
    std::cout << args[0].num << "\n";
  }
  else
  {
    std::cout << args[0].mat;
  }
  return RuntimeValue(0);
}

shirokov::RuntimeValue shirokov::builtinNewline(const Vector< RuntimeValue >& args)
{
  expectCount(args, 0, "newline");
  std::cout << "\n";
  return RuntimeValue(0);
}

shirokov::RuntimeValue shirokov::builtinZeros(const Vector< RuntimeValue >& args)
{
  expectCount(args, 2, "zeros");
  int n = asNumber(args[0], "zeros");
  int m = asNumber(args[1], "zeros");
  if (n <= 0 || m <= 0)
  {
    throw std::runtime_error("zeros(): dimensions must be positive");
  }
  return RuntimeValue(zeros(static_cast< size_t >(n), static_cast< size_t >(m)));
}

shirokov::RuntimeValue shirokov::builtinOnes(const Vector< RuntimeValue >& args)
{
  expectCount(args, 2, "ones");
  int n = asNumber(args[0], "ones");
  int m = asNumber(args[1], "ones");
  if (n <= 0 || m <= 0)
  {
    throw std::runtime_error("ones(): dimensions must be positive");
  }
  return RuntimeValue(ones(static_cast< size_t >(n), static_cast< size_t >(m)));
}

shirokov::RuntimeValue shirokov::builtinEye(const Vector< RuntimeValue >& args)
{
  expectCount(args, 1, "eye");
  int n = asNumber(args[0], "eye");
  if (n <= 0)
  {
    throw std::runtime_error("eye(): dimension must be positive");
  }
  return RuntimeValue(eye(static_cast< size_t >(n)));
}

shirokov::RuntimeValue shirokov::builtinFill(const Vector< RuntimeValue >& args)
{
  expectCount(args, 3, "fill");
  int n = asNumber(args[0], "fill");
  int m = asNumber(args[1], "fill");
  int k = asNumber(args[2], "fill");
  if (n <= 0 || m <= 0)
  {
    throw std::runtime_error("fill(): dimensions must be positive");
  }
  return RuntimeValue(fill(static_cast< size_t >(n), static_cast< size_t >(m), k));
}

shirokov::RuntimeValue shirokov::builtinDet(const Vector< RuntimeValue >& args)
{
  expectCount(args, 1, "det");
  const Matrix& m = asMatrix(args[0], "det");
  return RuntimeValue(m.det());
}

shirokov::RuntimeValue shirokov::builtinTr(const Vector< RuntimeValue >& args)
{
  expectCount(args, 1, "tr");
  const Matrix& m = asMatrix(args[0], "tr");
  return RuntimeValue(m.tr());
}

shirokov::RuntimeValue shirokov::builtinTranspose(const Vector< RuntimeValue >& args)
{
  expectCount(args, 1, "transpose");
  return RuntimeValue(transpose(asMatrix(args[0], "transpose")));
}

shirokov::RuntimeValue shirokov::builtinConcatH(const Vector< RuntimeValue >& args)
{
  expectCount(args, 2, "concatH");
  return RuntimeValue(concatH(asMatrix(args[0], "concatH"), asMatrix(args[1], "concatH")));
}

shirokov::RuntimeValue shirokov::builtinConcatV(const Vector< RuntimeValue >& args)
{
  expectCount(args, 2, "concatV");
  return RuntimeValue(concatV(asMatrix(args[0], "concatV"), asMatrix(args[1], "concatV")));
}

shirokov::RuntimeValue shirokov::builtinConcatDiag(const Vector< RuntimeValue >& args)
{
  expectCount(args, 2, "concatDiag");
  return RuntimeValue(concatDiag(asMatrix(args[0], "concatDiag"), asMatrix(args[1], "concatDiag")));
}
