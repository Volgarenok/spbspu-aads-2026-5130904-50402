#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <cstddef>

namespace novikov
{
  class Matrix
  {
  private:
    size_t rows;
    size_t cols;
    double *data;

  public:
    Matrix();
    Matrix(size_t rows, size_t cols);
    Matrix(size_t rows, size_t cols, const double *in);
    Matrix(const Matrix &other);
    Matrix(Matrix &&other) noexcept;
    ~Matrix();
    Matrix &operator=(const Matrix &other);
    Matrix &operator=(Matrix &&other) noexcept;
    size_t getRows() const;
    size_t getCols() const;
    double &at(size_t i, size_t j);
    const double &at(size_t i, size_t j) const;
    void change(size_t i, size_t j, double value);
    void addRow(size_t index, const double *values, size_t size);
    void addCol(size_t index, const double *values, size_t size);
    void swapRows(size_t i1, size_t i2);
    void swapCols(size_t i1, size_t i2);
    void deleteRow(size_t index);
    void deleteCol(size_t index);
    Matrix add(const Matrix &other) const;
    Matrix subtract(const Matrix &other) const;
    Matrix multiply(const Matrix &other) const;
    Matrix scalarMultiply(double scalar) const;
    Matrix transpose() const;
    double determinant() const;
    bool luDecomposition(Matrix &L, Matrix &U) const;
    double *solveSystem(const double *b) const;
    void print() const;
  };
}

#endif
