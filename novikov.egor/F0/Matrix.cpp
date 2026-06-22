#include "Matrix.hpp"
#include <iostream>
#include <stdexcept>

namespace novikov
{
  Matrix::Matrix():
    rows(0),
    cols(0),
    data(nullptr)
  {}

  Matrix::Matrix(size_t rows, size_t cols):
    rows(rows),
    cols(cols),
    data(rows > 0 && cols > 0 ? new double[rows * cols]() : nullptr)
  {
    if (data) {
      for (size_t i = 0; i < rows * cols; ++i) {
        data[i] = 0;
      }
    }
  }

  Matrix::Matrix(size_t rows, size_t cols, const double *in):
    rows(rows),
    cols(cols),
    data(rows > 0 && cols > 0 ? new double[rows * cols]() : nullptr)
  {
    if (data && in) {
      for (size_t i = 0; i < rows * cols; ++i) {
        data[i] = in[i];
      }
    }
  }

  Matrix::Matrix(const Matrix &other):
    rows(other.rows),
    cols(other.cols)
  {
    if (rows > 0 && cols > 0) {
      data = new double[rows * cols];
      for (size_t i = 0; i < rows * cols; ++i) {
        data[i] = other.data[i];
      }
    } else {
      data = nullptr;
    }
  }

  Matrix::Matrix(Matrix &&other) noexcept:
    rows(other.rows),
    cols(other.cols),
    data(other.data)
  {
    other.rows = 0;
    other.cols = 0;
    other.data = nullptr;
  }

  Matrix::~Matrix()
  {
    delete[] data;
  }

  Matrix &Matrix::operator=(const Matrix &other)
  {
    if (this != &other) {
      double *newData = nullptr;

      if (other.rows > 0 && other.cols > 0) {
        newData = new double[other.rows * other.cols];
        for (size_t i = 0; i < other.rows * other.cols; ++i) {
          newData[i] = other.data[i];
        }
      }

      delete[] data;
      rows = other.rows;
      cols = other.cols;
      data = newData;
    }
    return *this;
  }

  Matrix &Matrix::operator=(Matrix &&other) noexcept
  {
    if (this != &other) {
      delete[] data;
      rows = other.rows;
      cols = other.cols;
      data = other.data;
      other.rows = 0;
      other.cols = 0;
      other.data = nullptr;
    }
    return *this;
  }

  size_t Matrix::getRows() const
  {
    return rows;
  }

  size_t Matrix::getCols() const
  {
    return cols;
  }

  double &Matrix::at(size_t i, size_t j)
  {
    if (i >= rows || j >= cols) {
      throw std::out_of_range("Index out of range");
    }
    return data[i * cols + j];
  }

  const double &Matrix::at(size_t i, size_t j) const
  {
    if (i >= rows || j >= cols) {
      throw std::out_of_range("Index out of range");
    }
    return data[i * cols + j];
  }

  void Matrix::change(size_t i, size_t j, double value)
  {
    at(i, j) = value;
  }

  void Matrix::addRow(size_t index, const double *values, size_t size)
  {
    if (index > rows) {
      throw std::out_of_range("Row index out of bounds");
    }
    if (!values) {
      throw std::logic_error("Null pointer");
    }
    if (cols == 0) {
      throw std::logic_error("Cannot add row to matrix with 0 columns");
    }
    if (size != cols) {
      throw std::logic_error("Invalid size of elements");
    }
    double *newData = new double[(rows + 1) * cols]();
    try {
      for (size_t i = 0; i < index; ++i) {
        for (size_t j = 0; j < cols; ++j) {
          newData[i * cols + j] = data[i * cols + j];
        }
      }
      for (size_t j = 0; j < cols; ++j) {
        newData[index * cols + j] = values[j];
      }
      for (size_t i = index + 1; i <= rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
          newData[i * cols + j] = data[(i - 1) * cols + j];
        }
      }
    } catch (...) {
      delete[] newData;
      throw;
    }
    delete[] data;
    data = newData;
    rows++;
  }

  void Matrix::addCol(size_t index, const double *values, size_t size)
  {
    if (index > cols) {
      throw std::out_of_range("Column index out of bounds");
    }
    if (!values) {
      throw std::logic_error("Null pointer");
    }
    if (rows == 0) {
      throw std::logic_error("Cannot add column to matrix with 0 rows");
    }
    if (size != rows) {
      throw std::logic_error("Invalid number of elements");
    }
    double *newData = new double[rows * (cols + 1)]();
    try {
      for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < index; ++j) {
          newData[i * (cols + 1) + j] = data[i * cols + j];
        }
        newData[i * (cols + 1) + index] = values[i];
        for (size_t j = index + 1; j <= cols; ++j) {
          newData[i * (cols + 1) + j] = data[i * cols + j - 1];
        }
      }
    } catch (...) {
      delete[] newData;
      throw;
    }
    delete[] data;
    data = newData;
    cols++;
  }

  void Matrix::swapRows(size_t i1, size_t i2)
  {
    if (i1 >= rows || i2 >= rows) {
      throw std::out_of_range("Row index out of bounds");
    }
    for (size_t j = 0; j < cols; ++j) {
      double temp = data[i1 * cols + j];
      data[i1 * cols + j] = data[i2 * cols + j];
      data[i2 * cols + j] = temp;
    }
  }

  void Matrix::swapCols(size_t j1, size_t j2)
  {
    if (j1 >= cols || j2 >= cols) {
      throw std::out_of_range("Column index out of bounds");
    }
    for (size_t i = 0; i < rows; ++i) {
      double temp = data[i * cols + j1];
      data[i * cols + j1] = data[i * cols + j2];
      data[i * cols + j2] = temp;
    }
  }

  void Matrix::deleteRow(size_t index)
  {
    if (index >= rows) {
      throw std::out_of_range("Row index out of bounds");
    }
    if (rows == 0) {
      throw std::logic_error("Cannot delete from empty matrix");
    }
    if (rows == 1) {
      delete[] data;
      data = nullptr;
      rows = 0;
      cols = 0;
      return;
    }
    double *newData = (rows - 1 > 0 && cols > 0) ? new double[(rows - 1) * cols]() : nullptr;
    if (newData) {
      for (size_t i = 0, newI = 0; i < rows; ++i) {
        if (i == index)
          continue;
        for (size_t j = 0; j < cols; ++j) {
          newData[newI * cols + j] = data[i * cols + j];
        }
        newI++;
      }
    }
    delete[] data;
    data = newData;
    rows--;
  }

  void Matrix::deleteCol(size_t index)
  {
    if (index >= cols) {
      throw std::out_of_range("Column index out of bounds");
    }
    if (cols == 0) {
      throw std::logic_error("Cannot delete from empty matrix");
    }
    if (cols == 1) {
      delete[] data;
      data = nullptr;
      rows = 0;
      cols = 0;
      return;
    }
    double *newData = (rows > 0 && cols - 1 > 0) ? new double[rows * (cols - 1)]() : nullptr;
    if (newData) {
      for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0, newJ = 0; j < cols; ++j) {
          if (j == index)
            continue;
          newData[i * (cols - 1) + newJ] = data[i * cols + j];
          newJ++;
        }
      }
    }
    delete[] data;
    data = newData;
    cols--;
  }

  Matrix Matrix::add(const Matrix &other) const
  {
    if (rows != other.rows || cols != other.cols) {
      throw std::logic_error("Matrices have different sizes");
    }
    Matrix result(rows, cols);
    for (size_t i = 0; i < rows * cols; ++i) {
      result.data[i] = data[i] + other.data[i];
    }
    return result;
  }

  Matrix Matrix::subtract(const Matrix &other) const
  {
    if (rows != other.rows || cols != other.cols) {
      throw std::logic_error("Matrices have different sizes");
    }
    Matrix result(rows, cols);
    for (size_t i = 0; i < rows * cols; ++i) {
      result.data[i] = data[i] - other.data[i];
    }
    return result;
  }

  Matrix Matrix::multiply(const Matrix &other) const
  {
    if (cols != other.rows) {
      throw std::logic_error("Invalid matrix sizes for multiplication");
    }
    Matrix result(rows, other.cols);
    for (size_t i = 0; i < rows; ++i) {
      for (size_t j = 0; j < other.cols; ++j) {
        double sum = 0.0;
        for (size_t k = 0; k < cols; ++k) {
          sum += data[i * cols + k] * other.data[k * other.cols + j];
        }
        result.data[i * other.cols + j] = sum;
      }
    }
    return result;
  }

  Matrix Matrix::scalarMultiply(double scalar) const
  {
    Matrix result(rows, cols);
    for (size_t i = 0; i < rows * cols; ++i) {
      result.data[i] = data[i] * scalar;
    }
    return result;
  }

  Matrix Matrix::transpose() const
  {
    Matrix result(cols, rows);
    for (size_t i = 0; i < rows; ++i) {
      for (size_t j = 0; j < cols; ++j) {
        result.data[j * rows + i] = data[i * cols + j];
      }
    }
    return result;
  }

  double Matrix::determinant() const
  {
    if (rows != cols) {
      throw std::logic_error("Matrix is not square");
    }
    if (rows == 1) {
      return data[0];
    }
    if (rows == 2) {
      return data[0] * data[3] - data[1] * data[2];
    }
    double det = 0.0;
    for (size_t j = 0; j < cols; ++j) {
      Matrix sub(rows - 1, cols - 1);
      for (size_t i = 1; i < rows; ++i) {
        for (size_t k = 0; k < cols; ++k) {
          if (k < j) {
            sub.data[(i - 1) * (cols - 1) + k] = data[i * cols + k];
          } else if (k > j) {
            sub.data[(i - 1) * (cols - 1) + k - 1] = data[i * cols + k];
          }
        }
      }
      double sign = (j % 2 == 0) ? 1.0 : -1.0;
      det += sign * data[j] * sub.determinant();
    }
    return det;
  }

  void Matrix::print() const
  {
    if (!data || rows == 0 || cols == 0) {
      return;
    }
    for (size_t i = 0; i < rows; ++i) {
      for (size_t j = 0; j < cols; ++j) {
        std::cout << data[i * cols + j] << " ";
      }
      std::cout << "\n";
    }
  }

  bool Matrix::luDecomposition(Matrix &L, Matrix &U) const
  {
    if (rows != cols) {
      throw std::logic_error("Matrix is not square");
    }
    L = Matrix(rows, cols);
    U = Matrix(rows, cols);
    for (size_t i = 0; i < rows; ++i) {
      for (size_t j = 0; j < cols; ++j) {
        L.at(i, j) = 0.0;
        U.at(i, j) = data[i * cols + j];
      }
    }
    for (size_t k = 0; k < rows; ++k) {
      if (std::abs(U.at(k, k)) < 1e-10) {
        return false;
      }
      L.at(k, k) = 1.0;
      for (size_t i = k + 1; i < rows; ++i) {
        double factor = U.at(i, k) / U.at(k, k);
        L.at(i, k) = factor;

        for (size_t j = k; j < cols; ++j) {
          U.at(i, j) -= factor * U.at(k, j);
        }
      }
    }
    return true;
  }

  double *Matrix::solveSystem(const double *b) const
  {
    if (rows != cols) {
      throw std::logic_error("Matrix is not square");
    }
    Matrix L, U;
    if (!luDecomposition(L, U)) {
      throw std::logic_error("Matrix is singular");
    }
    double *y = new double[rows]();
    for (size_t i = 0; i < rows; ++i) {
      double sum = 0.0;
      for (size_t j = 0; j < i; ++j) {
        sum += L.at(i, j) * y[j];
      }
      y[i] = b[i] - sum;
    }
    double *x = nullptr;
    try {
      x = new double[rows]();
    } catch (const std::bad_alloc &) {
      delete[] y;
      throw;
    }
    for (size_t i = rows; i > 0; --i) {
      size_t idx = i - 1;
      double sum = 0.0;
      for (size_t j = i; j < rows; ++j) {
        sum += U.at(idx, j) * x[j];
      }
      x[idx] = (y[idx] - sum) / U.at(idx, idx);
    }
    delete[] y;
    return x;
  }
}
