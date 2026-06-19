#ifndef MATRIX_HPP
#define MATRIX_HPP
#include <Vector.hpp>
#include <iosfwd>

namespace shirokov
{
  class Matrix
  {
  public:
    Matrix() = default;
    Matrix(const shirokov::Vector< shirokov::Vector< int > >&);

    int det() const;
    int tr() const;

    int& at(int, int);
    int at(int, int) const;

    size_t getRows() const;
    size_t getCols() const;
    bool empty() const;

    void addRow(const shirokov::Vector< int >&);
    void addCol(const shirokov::Vector< int >&);
    void delRow(int);
    void delCol(int);
    void resize(int, int, int = 0);

  private:
    shirokov::Vector< shirokov::Vector< int > > matrix;
  };

  Matrix operator+(const Matrix&, const Matrix&);
  Matrix operator-(const Matrix&, const Matrix&);
  Matrix operator*(const Matrix&, int);
  Matrix operator*(int, const Matrix&);
  Matrix operator*(const Matrix&, const Matrix&);
  Matrix transpose(const Matrix&);
  Matrix pow(const Matrix&, int);
  Matrix zeros(int, int);
  Matrix ones(int, int);
  Matrix eye(int);
  Matrix fill(int, int, int);
  Matrix concatH(const Matrix&, const Matrix&);
  Matrix concatV(const Matrix&, const Matrix&);
  Matrix concatDiag(const Matrix&, const Matrix&);

  std::ostream& operator<<(std::ostream&, const Matrix&);
  std::istream& operator>>(std::istream&, Matrix&);
}

#endif
