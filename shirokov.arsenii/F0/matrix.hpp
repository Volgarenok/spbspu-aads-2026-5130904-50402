#ifndef MATRIX_HPP
#define MATRIX_HPP
#include <Vector.hpp>
#include <istream>
#include <ostream>

namespace shirokov
{
  class Matrix
  {
  public:
    Matrix() = default;
    Matrix(const Vector< Vector< int > >&);

    int det() const;
    int tr() const;

    int& at(size_t, size_t);
    const int& at(size_t, size_t) const;

    size_t getRows() const;
    size_t getCols() const;
    bool empty() const;

    void addRow(const Vector< int >&);
    void addCol(const Vector< int >&);
    void delRow(size_t);
    void delCol(size_t);
    void resize(size_t, size_t, int = 0);

  private:
    Vector< Vector< int > > matrix_;
  };

  Matrix operator+(const Matrix&, const Matrix&);
  Matrix operator-(const Matrix&, const Matrix&);
  Matrix operator*(Matrix, int);
  Matrix operator*(int, const Matrix&);
  Matrix operator*(const Matrix&, const Matrix&);
  Matrix transpose(const Matrix&);
  Matrix pow(const Matrix&, int);
  Matrix zeros(size_t, size_t);
  Matrix ones(size_t, size_t);
  Matrix eye(size_t);
  Matrix fill(size_t, size_t, int);
  Matrix concatH(const Matrix&, const Matrix&);
  Matrix concatV(const Matrix&, const Matrix&);
  Matrix concatDiag(const Matrix&, const Matrix&);

  std::ostream& operator<<(std::ostream&, const Matrix&);
  std::istream& operator>>(std::istream&, Matrix&);
}

#endif
