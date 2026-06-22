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
    throw std::invalid_argument("Matrix dimensions must match for addition");
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
    throw std::invalid_argument("Matrix dimensions must match for subtraction");
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
    throw std::invalid_argument("Matrix must be square and non-empty for exponentiation");
  }
  if (k < 0)
  {
    throw std::invalid_argument("Exponent must be non-negative");
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
    throw std::invalid_argument("Matrix dimensions mismatch for multiplication");
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
    throw std::invalid_argument("Matrix must be square and non-empty for determinant calculation");
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

  shirokov::Vector< shirokov::Vector< double > > temp;
  temp.reserve(n);
  for (size_t i = 0; i < n; ++i)
  {
    shirokov::Vector< double > row;
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
    throw std::invalid_argument("Matrix must be square and non-empty for trace calculation");
  }
  int trace_val = 0;
  size_t n = getRows();
  for (size_t i = 0; i < n; ++i)
  {
    trace_val += matrix_[i][i];
  }
  return trace_val;
}
