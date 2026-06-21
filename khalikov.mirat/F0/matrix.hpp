#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <../common/top-it-vector.hpp>
#include <string>
#include <iostream>
#include <limits>

namespace khalikov
{
  struct Matrix
  {
    Matrix();
    Matrix(size_t r, size_t c, int b);
    Matrix(const Matrix &other);
    Matrix(Matrix &&other) noexcept;
    ~Matrix() = default;

    Matrix &operator=(const Matrix &rhs);
    Matrix &operator=(Matrix &&rhs) noexcept;

    Vector< long long > &operator[](size_t r);
    const Vector< long long > &operator[](size_t r) const;

    size_t getRows() const noexcept;
    size_t getCols() const noexcept;
    int getBase() const noexcept;

    void setBase(int newBase);
    void swap(Matrix &other) noexcept;

    long long stol(const std::string &str, int base) const;
    std::string ltos(long long num, int base) const;
    static void check(long long a, long long b, char op);

  private:
    size_t rows_;
    size_t cols_;
    int base_;
    Vector< Vector< long long > > mx_;
  };
}

#endif
