#ifndef ACTIONS_HPP
#define ACTIONS_HPP

#include <iosfwd>
#include <string>
#include "CuckooHash.hpp"
#include "Matrix.hpp"

namespace novikov
{
  using MatrixHash = CuckooHash< std::string, Matrix * >;
  void Create(std::istream &in, std::ostream &out, MatrixHash &db);
  void Show(std::istream &in, std::ostream &out, MatrixHash &db);
  void Delete(std::istream &in, std::ostream &out, MatrixHash &db);
  void Change(std::istream &in, std::ostream &out, MatrixHash &db);
  void AddRow(std::istream &in, std::ostream &out, MatrixHash &db);
  void AddCol(std::istream &in, std::ostream &out, MatrixHash &db);
  void SwapRows(std::istream &in, std::ostream &out, MatrixHash &db);
  void SwapCols(std::istream &in, std::ostream &out, MatrixHash &db);
  void DeleteRow(std::istream &in, std::ostream &out, MatrixHash &db);
  void DeleteCol(std::istream &in, std::ostream &out, MatrixHash &db);
  void Sum(std::istream &in, std::ostream &out, MatrixHash &db);
  void Sub(std::istream &in, std::ostream &out, MatrixHash &db);
  void Mul(std::istream &in, std::ostream &out, MatrixHash &db);
  void ScalarMul(std::istream &in, std::ostream &out, MatrixHash &db);
  void Transpose(std::istream &in, std::ostream &out, MatrixHash &db);
  void Det(std::istream &in, std::ostream &out, MatrixHash &db);
  void Lu(std::istream &in, std::ostream &out, MatrixHash &db);
  void Solve(std::istream &in, std::ostream &out, MatrixHash &db);
}

#endif
