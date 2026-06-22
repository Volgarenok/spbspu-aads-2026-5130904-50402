#include "matrix.hpp"
#include <Vector.hpp>
#include <cmath>
#include <stdexcept>

shirokov::Matrix::Matrix(const Vector< Vector< int > >& matrix):
  matrix_(matrix)
{}

size_t shirokov::Matrix::getRows() const
{
  return matrix_.getSize();
}

size_t shirokov::Matrix::getCols() const
{
  return (empty() ? 0 : matrix_[0].getSize());
}

bool shirokov::Matrix::empty() const
{
  return matrix_.isEmpty();
}

shirokov::Matrix shirokov::operator+(const Matrix& lhs, const Matrix& rhs)
{
  if (lhs.getCols() != rhs.getCols() || lhs.getRows() != rhs.getRows())
  {
    throw std::invalid_argument("Matrix error: matrix dimensions must match for addition");
  }
  Matrix res = zeros(lhs.getCols(), lhs.getRows());
  for (size_t i = 1; i <= lhs.getRows(); ++i)
  {
    for (size_t j = 1; j <= lhs.getCols(); ++j)
    {
      res.at(i, j) = lhs.at(i, j) + rhs.at(i, j);
    }
  }
  return res;
}

shirokov::Matrix shirokov::operator-(const Matrix& lhs, const Matrix& rhs)
{
  if (lhs.getCols() != rhs.getCols() || lhs.getRows() != rhs.getRows())
  {
    throw std::invalid_argument("Matrix error: matrix dimensions must match for subtraction");
  }
  Matrix res(lhs);
  for (size_t i = 1; i <= lhs.getRows(); ++i)
  {
    for (size_t j = 1; j <= lhs.getCols(); ++j)
    {
      res.at(i, j) -= rhs.at(i, j);
    }
  }
  return res;
}

shirokov::Matrix shirokov::pow(const Matrix& matrix, int k)
{
  if (matrix.empty() || matrix.getRows() != matrix.getCols())
  {
    throw std::invalid_argument("Matrix error: matrix must be square and non-empty for exponentiation");
  }
  if (k < 0)
  {
    throw std::invalid_argument("Matrix error: exponent must be non-negative");
  }
  if (k == 0)
  {
    return eye(matrix.getRows());
  }

  Matrix res = eye(matrix.getRows());
  Matrix base = matrix;
  long long tempK = k;
  while (tempK > 0)
  {
    if (tempK % 2 == 1)
    {
      res = res * base;
      tempK--;
    }
    if (tempK > 0)
    {
      base = base * base;
      tempK /= 2;
    }
  }
  return res;
}

shirokov::Matrix shirokov::operator*(Matrix matrix, int k)
{
  for (size_t i = 1; i <= matrix.getRows(); ++i)
  {
    for (size_t j = 1; j <= matrix.getCols(); ++j)
    {
      matrix.at(i, j) *= k;
    }
  }
  return matrix;
}

shirokov::Matrix shirokov::operator*(int k, const Matrix& matrix)
{
  return matrix * k;
}

shirokov::Matrix shirokov::operator*(const Matrix& lhs, const Matrix& rhs)
{
  if (lhs.getCols() != rhs.getRows())
  {
    throw std::invalid_argument("Matrix error: matrix dimensions mismatch for multiplication");
  }

  Matrix res = zeros(lhs.getRows(), rhs.getCols());
  for (size_t i = 1; i <= lhs.getRows(); ++i)
  {
    for (size_t j = 1; j <= rhs.getCols(); ++j)
    {
      for (size_t k = 1; k <= lhs.getCols(); ++k)
      {
        res.at(i, j) += lhs.at(i, k) * rhs.at(k, j);
      }
    }
  }
  return res;
}

shirokov::Matrix shirokov::transpose(const Matrix& matrix)
{
  Matrix res = zeros(matrix.getCols(), matrix.getRows());
  for (size_t i = 1; i <= matrix.getRows(); ++i)
  {
    for (size_t j = 1; j <= matrix.getCols(); ++j)
    {
      res.at(j, i) = matrix.at(i, j);
    }
  }
  return res;
}

int shirokov::Matrix::det() const
{
  if (empty() || getRows() != getCols())
  {
    throw std::invalid_argument("Matrix error: matrix must be square and non-empty for determinant calculation");
  }

  size_t n = getRows();
  if (n == 1)
  {
    return matrix_[0][0];
  }
  if (n == 2)
  {
    return matrix_[0][0] * matrix_[1][1] - matrix_[0][1] * matrix_[1][0];
  }

  Vector< Vector< double > > temp;
  temp.reserve(n);
  for (size_t i = 0; i < n; ++i)
  {
    Vector< double > row;
    row.reserve(n);
    for (size_t j = 0; j < n; ++j)
    {
      row.pushBack(static_cast< double >(matrix_[i][j]));
    }
    temp.pushBack(row);
  }

  double detVal = 1;
  for (size_t i = 0; i < n; ++i)
  {
    size_t pivot = i;
    for (size_t j = i + 1; j < n; ++j)
    {
      if (std::abs(temp[j][i]) > std::abs(temp[pivot][i]))
      {
        pivot = j;
      }
    }
    if (std::abs(temp[pivot][i]) < 1e-9)
    {
      return 0;
    }
    if (pivot != i)
    {
      temp[i].swap(temp[pivot]);
      detVal = -detVal;
    }
    detVal *= temp[i][i];
    for (size_t j = i + 1; j < n; ++j)
    {
      double factor = temp[j][i] / temp[i][i];
      for (size_t k = i + 1; k < n; ++k)
      {
        temp[j][k] -= factor * temp[i][k];
      }
    }
  }
  return static_cast< int >(std::round(detVal));
}

int shirokov::Matrix::tr() const
{
  if (empty() || getRows() != getCols())
  {
    throw std::invalid_argument("Matrix error: matrix must be square and non-empty for trace calculation");
  }
  int traceVal = 0;
  size_t n = getRows();
  for (size_t i = 0; i < n; ++i)
  {
    traceVal += matrix_[i][i];
  }
  return traceVal;
}

shirokov::Matrix shirokov::zeros(size_t n, size_t m)
{
  return fill(n, m, 0);
}

shirokov::Matrix shirokov::ones(size_t n, size_t m)
{
  return fill(n, m, 1);
}

shirokov::Matrix shirokov::eye(size_t n)
{
  if (n == 0)
  {
    throw std::invalid_argument("Matrix error: identity matrix size must be greater than zero");
  }
  Vector< Vector< int > > mat;
  mat.reserve(n);
  for (size_t i = 0; i < n; ++i)
  {
    Vector< int > row;
    row.reserve(n);
    for (size_t j = 0; j < n; ++j)
    {
      row.pushBack(i == j ? 1 : 0);
    }

    mat.pushBack(row);
  }
  return Matrix(mat);
}

shirokov::Matrix shirokov::fill(size_t n, size_t m, int k)
{
  if (n == 0 || m == 0)
  {
    throw std::invalid_argument("Matrix error: matrix dimensions must be greater than zero");
  }
  Vector< Vector< int > > mat;
  mat.reserve(n);
  for (size_t i = 0; i < n; ++i)
  {
    Vector< int > row;
    row.reserve(m);
    for (size_t j = 0; j < m; ++j)
    {
      row.pushBack(k);
    }
    mat.pushBack(row);
  }
  return Matrix(mat);
}

const int& shirokov::Matrix::at(size_t r, size_t c) const
{
  if (r == 0 || r > getRows() || c == 0 || c > getCols() || empty())
  {
    throw std::out_of_range("Matrix error: matrix index out of bounds");
  }
  return matrix_[r - 1][c - 1];
}

int& shirokov::Matrix::at(size_t r, size_t c)
{
  return const_cast< int& >(static_cast< const Matrix* >(this)->at(r, c));
}

void shirokov::Matrix::addRow(const Vector< int >& row)
{
  if (!empty() && row.getSize() != getCols())
  {
    throw std::invalid_argument("Matrix error: row size does not match matrix column count");
  }
  matrix_.pushBack(row);
}

void shirokov::Matrix::addCol(const Vector< int >& col)
{
  if (empty())
  {
    matrix_.reserve(col.getSize());
    for (size_t i = 0; i < col.getSize(); ++i)
    {
      Vector< int > newRow;
      newRow.pushBack(col[i]);
      matrix_.pushBack(newRow);
    }
  }
  else
  {
    if (col.getSize() != getRows())
    {
      throw std::invalid_argument("Matrix error: column size does not match matrix row count");
    }
    for (size_t i = 0; i < getRows(); ++i)
    {
      matrix_[i].pushBack(col[i]);
    }
  }
}

void shirokov::Matrix::delRow(size_t pos)
{
  if (pos == 0 || pos > getRows() || empty())
  {
    throw std::out_of_range("Matrix error: row index out of bounds");
  }
  matrix_.erase(pos - 1);
}

void shirokov::Matrix::delCol(size_t pos)
{
  if (pos == 0 || pos > getCols() || empty())
  {
    throw std::out_of_range("Matrix error: column index out of bounds");
  }
  size_t rows = getRows();
  for (size_t i = 0; i < rows; ++i)
  {
    matrix_[i].erase(pos - 1);
  }
  if (matrix_[0].isEmpty())
  {
    matrix_ = Vector< Vector< int > >();
  }
}

void shirokov::Matrix::resize(size_t n, size_t m, int k)
{
  if (n == 0 || m == 0)
  {
    throw std::invalid_argument("Matrix error: invalid resize dimensions");
  }
  size_t currentRows = matrix_.getSize();
  for (size_t i = 0; i < currentRows; ++i)
  {
    while (matrix_[i].getSize() > m)
    {
      matrix_[i].popBack();
    }
    while (matrix_[i].getSize() < m)
    {
      matrix_[i].pushBack(k);
    }
  }
  while (matrix_.getSize() > n)
  {
    matrix_.popBack();
  }
  if (matrix_.getSize() < n)
  {
    matrix_.reserve(n);
    while (matrix_.getSize() < n)
    {
      Vector< int > newRow;
      newRow.reserve(m);
      for (size_t j = 0; j < m; ++j)
      {
        newRow.pushBack(k);
      }
      matrix_.pushBack(newRow);
    }
  }
}

std::istream& shirokov::operator>>(std::istream& in, Matrix& matrix)
{
  char ch = 0;
  if (!(in >> ch) || ch != '[')
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  Vector< Vector< int > > mat;
  Vector< int > currentRow;
  size_t expectedCols = 0;
  bool matrixDone = false;
  while (!matrixDone && in >> std::ws)
  {
    if (in.peek() == ']')
    {
      in >> ch;
      matrixDone = true;
      break;
    }
    int val = 0;
    if (!(in >> val))
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    currentRow.pushBack(val);
    in >> std::ws;
    int nextChar = in.peek();
    if (nextChar == ',')
    {
      in >> ch;
    }
    else if (nextChar == ';')
    {
      in >> ch;
      if (expectedCols == 0)
      {
        expectedCols = currentRow.getSize();
      }
      else if (currentRow.getSize() != expectedCols)
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      mat.pushBack(currentRow);
      currentRow = Vector< int >();
    }
    else if (nextChar == ']')
    {
      in >> ch;
      matrixDone = true;
      if (expectedCols == 0)
      {
        expectedCols = currentRow.getSize();
      }
      else if (currentRow.getSize() != expectedCols)
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      mat.pushBack(currentRow);
    }
    else
    {
      in.setstate(std::ios::failbit);
      return in;
    }
  }
  if (!matrixDone || mat.isEmpty() || expectedCols == 0)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  matrix.resize(mat.getSize(), expectedCols);
  for (size_t i = 0; i < mat.getSize(); ++i)
  {
    for (size_t j = 0; j < expectedCols; ++j)
    {
      matrix.at(i + 1, j + 1) = mat[i][j];
    }
  }
  return in;
}

std::ostream& shirokov::operator<<(std::ostream& out, const Matrix& matrix)
{
  if (matrix.empty())
  {
    return out;
  }
  size_t rows = matrix.getRows();
  size_t cols = matrix.getCols();
  for (size_t i = 1; i <= rows; ++i)
  {
    for (size_t j = 1; j <= cols; ++j)
    {
      out << matrix.at(i, j);
      if (j < cols)
      {
        out << " ";
      }
    }
    out << "\n";
  }
  return out;
}

shirokov::Matrix shirokov::concatH(const Matrix& lhs, const Matrix& rhs)
{
  if (lhs.empty() || rhs.empty())
  {
    throw std::invalid_argument("Matrix error: cannot concatenate empty matrices");
  }
  if (lhs.getRows() != rhs.getRows())
  {
    throw std::invalid_argument(
        "Matrix error: horizontal concatenation requires matrices to have the same number of rows");
  }

  size_t rows = lhs.getRows();
  size_t lCols = lhs.getCols();
  size_t rCols = rhs.getCols();
  Vector< Vector< int > > mat;
  mat.reserve(rows);
  for (size_t i = 1; i <= rows; ++i)
  {
    Vector< int > row;
    row.reserve(lCols + rCols);
    for (size_t j = 1; j <= lCols; ++j)
    {
      row.pushBack(lhs.at(i, j));
    }
    for (size_t j = 1; j <= rCols; ++j)
    {
      row.pushBack(rhs.at(i, j));
    }
    mat.pushBack(row);
  }
  return Matrix(mat);
}

shirokov::Matrix shirokov::concatV(const Matrix& lhs, const Matrix& rhs)
{
  if (lhs.empty() || rhs.empty())
  {
    throw std::invalid_argument("Matrix error: cannot concatenate empty matrices");
  }
  if (lhs.getCols() != rhs.getCols())
  {
    throw std::invalid_argument(
        "Matrix error: vertical concatenation requires matrices to have the same number of columns");
  }

  size_t lRows = lhs.getRows();
  size_t rRows = rhs.getRows();
  size_t cols = lhs.getCols();
  Vector< Vector< int > > mat;
  mat.reserve(lRows + rRows);
  for (size_t i = 1; i <= lRows; ++i)
  {
    Vector< int > row;
    row.reserve(cols);
    for (size_t j = 1; j <= cols; ++j)
    {
      row.pushBack(lhs.at(i, j));
    }
    mat.pushBack(row);
  }
  for (size_t i = 1; i <= rRows; ++i)
  {
    Vector< int > row;
    row.reserve(cols);
    for (size_t j = 1; j <= cols; ++j)
    {
      row.pushBack(rhs.at(i, j));
    }
    mat.pushBack(row);
  }
  return Matrix(mat);
}

shirokov::Matrix shirokov::concatDiag(const Matrix& lhs, const Matrix& rhs)
{
  if (lhs.empty() || rhs.empty())
  {
    throw std::invalid_argument("Matrix error: cannot concatenate empty matrices");
  }

  size_t lRows = lhs.getRows();
  size_t lCols = lhs.getCols();
  size_t rRows = rhs.getRows();
  size_t rCols = rhs.getCols();
  Vector< Vector< int > > mat;
  mat.reserve(lRows + rRows);
  for (size_t i = 1; i <= lRows; ++i)
  {
    Vector< int > row;
    row.reserve(lCols + rCols);
    for (size_t j = 1; j <= lCols; ++j)
    {
      row.pushBack(lhs.at(i, j));
    }
    for (size_t j = 1; j <= rCols; ++j)
    {
      row.pushBack(0);
    }
    mat.pushBack(row);
  }
  for (size_t i = 1; i <= rRows; ++i)
  {
    Vector< int > row;
    row.reserve(lCols + rCols);

    for (size_t j = 1; j <= lCols; ++j)
    {
      row.pushBack(0);
    }
    for (size_t j = 1; j <= rCols; ++j)
    {
      row.pushBack(rhs.at(i, j));
    }
    mat.pushBack(row);
  }
  return Matrix(mat);
}
