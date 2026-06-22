#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <Vector.hpp>

namespace shirokov
{
  struct RuntimeValue;

  RuntimeValue builtinPrint(const Vector< RuntimeValue >&);
  RuntimeValue builtinNewline(const Vector< RuntimeValue >&);
  RuntimeValue builtinZeros(const Vector< RuntimeValue >&);
  RuntimeValue builtinOnes(const Vector< RuntimeValue >&);
  RuntimeValue builtinEye(const Vector< RuntimeValue >&);
  RuntimeValue builtinFill(const Vector< RuntimeValue >&);
  RuntimeValue builtinDet(const Vector< RuntimeValue >&);
  RuntimeValue builtinTr(const Vector< RuntimeValue >&);
  RuntimeValue builtinTranspose(const Vector< RuntimeValue >&);
  RuntimeValue builtinConcatH(const Vector< RuntimeValue >&);
  RuntimeValue builtinConcatV(const Vector< RuntimeValue >&);
  RuntimeValue builtinConcatDiag(const Vector< RuntimeValue >&);
}

#endif
