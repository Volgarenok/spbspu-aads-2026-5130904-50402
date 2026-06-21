#include "matrix.hpp"
#include <limits>
#include <stdexcept>

namespace khalikov
{
  void Matrix::check(long long a, long long b, char op)
  {
    switch (op) {
    case '+': {
      if ((b > 0) && (a > std::numeric_limits< long long >::max() - b)) {
        throw std::overflow_error("Overflow");
      }
      if ((b < 0) && (a < std::numeric_limits< long long >::min() - b)) {
        throw std::underflow_error("Underflow");
      }
      break;
    }
    case '-': {
      if ((b < 0) && (a > std::numeric_limits< long long >::max() + b)) {
        throw std::overflow_error("Overflow");
      }
      if ((b > 0) && (a < std::numeric_limits< long long >::min() + b)) {
        throw std::underflow_error("Underflow");
      }
      break;
    }
    case '*': {
      if (a == 0 || b == 0) {
        return;
      }
      if (((a > 0 && b > 0) && (a > std::numeric_limits< long long >::max() / b))
          || ((a < 0 && b < 0) && (b < std::numeric_limits< long long >::max() / a))) {
        throw std::overflow_error("Overflow");
      }
      if (((a > 0 && b < 0) && (b < std::numeric_limits< long long >::min() / a))
          || ((a < 0 && b > 0) && (a < std::numeric_limits< long long >::min() / b))) {
        throw std::underflow_error("Underflow");
      }
      break;
    }
    default:
      throw std::invalid_argument("Unknown op");
    }
  }

  long long Matrix::stol(const std::string &str, int base) const
  {
    if (str.empty()) {
      throw std::invalid_argument("Empty string");
    }
    long long res = 0;
    bool isNegative = (str[0] == '-');
    size_t i = (isNegative || str[0] == '+') ? 1 : 0;
    if (i == str.size()) {
      throw std::invalid_argument("Invalid number format");
    }
    for (; i < str.size(); ++i) {
      char ch = str[i];
      int digit = 0;
      switch (ch) {
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        digit = ch - '0';
        break;
      case 'A':
      case 'B':
      case 'C':
      case 'D':
      case 'E':
      case 'F':
      case 'G':
      case 'H':
      case 'I':
      case 'J':
      case 'K':
      case 'L':
      case 'M':
      case 'N':
      case 'O':
      case 'P':
      case 'Q':
      case 'R':
      case 'S':
      case 'T':
      case 'U':
      case 'V':
      case 'W':
      case 'X':
      case 'Y':
      case 'Z':
        digit = ch - 'A' + 10;
        break;
      case 'a':
      case 'b':
      case 'c':
      case 'd':
      case 'e':
      case 'f':
      case 'g':
      case 'h':
      case 'i':
      case 'j':
      case 'k':
      case 'l':
      case 'm':
      case 'n':
      case 'o':
      case 'p':
      case 'q':
      case 'r':
      case 's':
      case 't':
      case 'u':
      case 'v':
      case 'w':
      case 'x':
      case 'y':
      case 'z':
        digit = ch - 'a' + 10;
        break;
      default:
        throw std::invalid_argument("Invalid character");
      }
      if (digit >= base) {
        throw std::invalid_argument("Input error");
      }
      check(res, base, '*');
      res *= base;
      check(res, digit, '+');
      res += digit;
    }
    return isNegative ? -res : res;
  }

  std::string Matrix::ltos(long long num, int base) const
  {
    if (num == 0) {
      return "0";
    }
    std::string result = "";
    bool isNegative = (num < 0);
    size_t absoluteNum = isNegative ? -num : num;
    while (absoluteNum > 0) {
      int remainder = absoluteNum % base;
      if (remainder < 10) {
        result += (remainder + '0');
      } else {
        result += (remainder - 10 + 'A');
      }
      absoluteNum /= base;
    }
    if (isNegative) {
      result += '-';
    }
    size_t n = result.length();
    for (size_t i = 0; i < n / 2; ++i) {
      std::swap(result[i], result[n - i - 1]);
    }
    return result;
  }

  void Matrix::swap(Matrix &other) noexcept
  {
    std::swap(rows_, other.rows_);
    std::swap(cols_, other.cols_);
    std::swap(base_, other.base_);
    std::swap(mx_, other.mx_);
  }

  Matrix::Matrix():
    rows_(0),
    cols_(0),
    base_(10),
    mx_()
  {}

  Matrix::Matrix(size_t r, size_t c, int b):
    rows_(r),
    cols_(c),
    base_(b),
    mx_()
  {
    for (size_t i = 0; i < r; ++i) {
      Vector< long long > row;
      for (size_t j = 0; j < c; ++j) {
        row.pushBack(0);
      }
      mx_.pushBack(std::move(row));
    }
  }

  Matrix::Matrix(const Matrix &other):
    rows_(other.rows_),
    cols_(other.cols_),
    base_(other.base_),
    mx_(other.mx_)
  {}

  Matrix::Matrix(Matrix &&other) noexcept:
    rows_(0),
    cols_(0),
    base_(10),
    mx_()
  {
    swap(other);
  }

  Matrix &khalikov::Matrix::operator=(const Matrix &rhs)
  {
    if (this != std::addressof(rhs)) {
      Matrix temp(rhs);
      swap(temp);
    }
    return *this;
  }

  Matrix &khalikov::Matrix::operator=(Matrix &&rhs) noexcept
  {
    if (this != std::addressof(rhs)) {
      rows_ = 0;
      cols_ = 0;
      base_ = 10;
      mx_ = Vector< Vector< long long > >();
      swap(rhs);
    }
    return *this;
  }

  Vector< long long > &khalikov::Matrix::operator[](size_t r)
  {
    return mx_[r];
  }

  const Vector< long long > &khalikov::Matrix::operator[](size_t r) const
  {
    return mx_[r];
  }

  size_t Matrix::getRows() const noexcept
  {
    return rows_;
  }

  size_t Matrix::getCols() const noexcept
  {
    return cols_;
  }

  int Matrix::getBase() const noexcept
  {
    return base_;
  }

  void Matrix::setBase(int newBase)
  {
    base_ = newBase;
  }
}
