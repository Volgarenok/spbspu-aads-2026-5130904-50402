#include "commands.hpp"
#include <sstream>

namespace khalikov
{
  const khalikov::Matrix *find(const Storage &storage, const std::string &name)
  {
    for (auto it = storage.cbegin(); it != storage.cend(); ++it) {
      if ((*it).first == name) {
        return &((*it).second);
      }
    }
    return nullptr;
  }

  khalikov::Matrix *find(Storage &storage, const std::string &name)
  {
    for (auto it = storage.begin(); it != storage.end(); ++it) {
      if ((*it).first == name) {
        return &((*it).second);
      }
    }
    return nullptr;
  }

  void checkInput(std::istream &in)
  {
    while (in.peek() != '\n' && in.peek() != EOF) {
      char ch = in.peek();
      if (!std::isspace(ch)) {
        throw std::invalid_argument("Signature of command is failed");
      }
      in.get();
    }
    if (in.peek() == '\n') {
      in.get();
    }
  }

  long long detImpl(const khalikov::Matrix &matrix, int &sign)
  {
    size_t n = matrix.getRows();
    khalikov::Vector< khalikov::Vector< long long > > a;
    a.resize(n);
    for (size_t i = 0; i < n; ++i) {
      a[i].resize(n);
      for (size_t j = 0; j < n; ++j) {
        a[i][j] = matrix[i][j];
      }
    }
    long long prevPivot = 1;
    for (size_t i = 0; i < n; ++i) {
      size_t maxRow = i;
      for (size_t k = i + 1; k < n; ++k) {
        if (std::abs(a[k][i]) > std::abs(a[maxRow][i])) {
          maxRow = k;
        }
      }
      if (a[maxRow][i] == 0) {
        return 0;
      }
      if (maxRow != i) {
        std::swap(a[i], a[maxRow]);
        sign = -sign;
      }
      long long pivot = a[i][i];
      for (size_t k = i + 1; k < n; ++k) {
        for (size_t j = i + 1; j < n; ++j) {
          a[k][j] = (pivot * a[k][j] - a[k][i] * a[i][j]) / prevPivot;
        }
      }
      prevPivot = pivot;
    }
    return a[n - 1][n - 1];
  }

  void det(std::ostream &out, std::istream &in, Storage &storage)
  {
    std::string name;
    if (!(in >> name)) {
      throw std::invalid_argument("Input error");
    }
    checkInput(in);
    const Matrix *matrix = find(storage, name);
    if (!matrix) {
      throw std::runtime_error("Matrix not found");
    }
    size_t n = matrix->getRows();
    if (n != matrix->getCols()) {
      throw std::invalid_argument("Matrix must be square");
    }
    if (n == 1) {
      out << (*matrix)[0][0] << "\n";
      return;
    }
    int sign = 1;
    long long detVal = detImpl(*matrix, sign);
    long long result = detVal * sign;
    out << result << "\n";
  }

  void create(std::ostream &, std::istream &in, Storage &storage)
  {
    std::string line;
    if (!std::getline(in, line)) {
      throw std::invalid_argument("Input error");
    }
    std::stringstream ss(line);
    std::string name;
    size_t rows = 0, cols = 0;
    int base = 10;
    if (!(ss >> name >> rows >> cols)) {
      throw std::invalid_argument("Input error");
    }
    ss >> base;
    checkInput(ss);
    if (find(storage, name) != nullptr) {
      throw std::runtime_error("Already exists");
    }
    khalikov::Matrix matrix(rows, cols, base);
    storage.insert(std::make_pair(name, std::move(matrix)));
  }

  void fill(std::ostream &, std::istream &in, Storage &storage)
  {
    std::string name;
    if (!(in >> name)) {
      throw std::invalid_argument("Input error");
    }
    Matrix *matrix = find(storage, name);
    if (!matrix) {
      throw std::runtime_error("Unknown matrix");
    }
    size_t r = matrix->getRows();
    size_t c = matrix->getCols();
    int base = matrix->getBase();
    for (size_t i = 0; i < r; ++i) {
      for (size_t j = 0; j < c; ++j) {
        std::string token;
        if (!(in >> token)) {
          throw std::invalid_argument("Input error");
        }
        (*matrix)[i][j] = matrix->stol(token, base);
      }
    }
    checkInput(in);
  }

  void show(std::ostream &out, std::istream &in, Storage &storage)
  {
    std::string name;
    if (!(in >> name)) {
      throw std::invalid_argument("Input error");
    }
    const Matrix *matrix = find(storage, name);
    if (!matrix) {
      throw std::runtime_error("Unknown matrix");
    }
    checkInput(in);
    size_t r = matrix->getRows();
    size_t c = matrix->getCols();
    int base = matrix->getBase();
    for (size_t i = 0; i < r; ++i) {
      for (size_t j = 0; j < c; ++j) {
        out << matrix->ltos((*matrix)[i][j], base);
        if (j + 1 < c) {
          out << " ";
        }
      }
      out << "\n";
    }
  }

  void add(std::ostream &, std::istream &in, Storage &storage)
  {
    std::string resName, m1Name, m2Name;
    int base = 10;
    if (!(in >> resName >> base >> m1Name >> m2Name)) {
      throw std::invalid_argument("Input error");
    }
    checkInput(in);
    const Matrix *m1 = find(storage, m1Name);
    const Matrix *m2 = find(storage, m2Name);
    if (!m1 || !m2) {
      throw std::runtime_error("Unknown matrix");
    }
    if (m1->getRows() != m2->getRows() || m1->getCols() != m2->getCols()) {
      throw std::runtime_error("Incompatible sizes");
    }
    size_t r = m1->getRows();
    size_t c = m1->getCols();
    Matrix result(r, c, base);
    for (size_t i = 0; i < r; ++i) {
      for (size_t j = 0; j < c; ++j) {
        Matrix::check((*m1)[i][j], (*m2)[i][j], '+');
        result[i][j] = (*m1)[i][j] + (*m2)[i][j];
      }
    }
    storage.insert(std::make_pair(resName, std::move(result)));
  }

  void sub(std::ostream &, std::istream &in, Storage &storage)
  {
    std::string resName, m1Name, m2Name;
    int base = 10;
    if (!(in >> resName >> base >> m1Name >> m2Name)) {
      throw std::invalid_argument("Input error");
    }
    checkInput(in);
    const Matrix *m1 = find(storage, m1Name);
    const Matrix *m2 = find(storage, m2Name);
    if (!m1 || !m2) {
      throw std::runtime_error("Unknown matrix");
    }
    if (m1->getRows() != m2->getRows() || m1->getCols() != m2->getCols()) {
      throw std::runtime_error("Incompatible sizes");
    }
    size_t r = m1->getRows();
    size_t c = m1->getCols();
    Matrix result(r, c, base);
    for (size_t i = 0; i < r; ++i) {
      for (size_t j = 0; j < c; ++j) {
        Matrix::check((*m1)[i][j], (*m2)[i][j], '-');
        result[i][j] = (*m1)[i][j] - (*m2)[i][j];
      }
    }
    storage.insert(std::make_pair(resName, std::move(result)));
  }

  void mul(std::ostream &, std::istream &in, Storage &storage)
  {
    std::string resName, m1Name, m2Name;
    int base = 10;
    if (!(in >> resName >> base >> m1Name >> m2Name)) {
      throw std::invalid_argument("Input error");
    }
    checkInput(in);
    const Matrix *m1 = find(storage, m1Name);
    const Matrix *m2 = find(storage, m2Name);
    if (!m1 || !m2) {
      throw std::runtime_error("Matrix not found");
    }
    if (m1->getCols() != m2->getRows()) {
      throw std::runtime_error("Incompatible sizes for multiplication");
    }
    size_t r1 = m1->getRows();
    size_t c1 = m1->getCols();
    size_t c2 = m2->getCols();
    Matrix result(r1, c2, base);
    for (size_t i = 0; i < r1; ++i) {
      for (size_t k = 0; k < c1; ++k) {
        long long a = (*m1)[i][k];
        if (a == 0) {
          continue;
        }
        for (size_t j = 0; j < c2; ++j) {
          long long b = (*m2)[k][j];
          Matrix::check(a, b, '*');
          long long prod = a * b;
          Matrix::check(result[i][j], prod, '+');
          result[i][j] += prod;
        }
      }
    }
    storage.insert(std::make_pair(resName, std::move(result)));
  }

  void remove(std::ostream &out, std::istream &in, Storage &storage)
  {
    std::string name;
    if (!(in >> name)) {
      throw std::invalid_argument("Input error");
    }
    checkInput(in);
    const Matrix *mx = find(storage, name);
    if (!mx) {
      throw std::runtime_error("Matrix not found");
    }
    std::pair< std::string, Matrix > target(name, *mx);
    storage.remove(target);
    out << "<" << name << " WAS DELETED>\n";
  }

  void clear(std::ostream &, std::istream &in, Storage &storage)
  {
    checkInput(in);
    while (!storage.empty()) {
      storage.remove(*(storage.begin()));
    }
  }

  void list(std::ostream &out, std::istream &in, Storage &storage)
  {
    checkInput(in);
    if (storage.empty()) {
      out << "<EMPTY>\n";
      return;
    }
    for (auto it = storage.cbegin(); it != storage.cend(); ++it) {
      out << "<" << it->first << '\t' << it->second.getRows() << "x" << it->second.getCols() << ">\n";
    }
  }

  void basech(std::ostream &, std::istream &in, Storage &storage)
  {
    std::string name;
    int newBase = 10;
    if (!(in >> name >> newBase)) {
      throw std::invalid_argument("Input error");
    }
    checkInput(in);
    Matrix *matrix = find(storage, name);
    if (!matrix) {
      throw std::runtime_error("Matrix not found");
    }
    if (newBase < 2 || newBase > 36) {
      throw std::invalid_argument("Invalid base");
    }
    matrix->setBase(newBase);
  }

  void transpose(std::ostream &, std::istream &in, Storage &storage)
  {
    std::string name;
    if (!(in >> name)) {
      throw std::invalid_argument("Input error");
    }
    checkInput(in);
    Matrix *mx = find(storage, name);
    if (!mx) {
      throw std::runtime_error("Matrix not found");
    }
    size_t r = mx->getRows();
    size_t c = mx->getCols();
    Matrix result(c, r, mx->getBase());
    for (size_t i = 0; i < r; ++i) {
      for (size_t j = 0; j < c; ++j) {
        result[j][i] = (*mx)[i][j];
      }
    }
    mx->swap(result);
  }

  void scale(std::ostream &, std::istream &in, Storage &storage)
  {
    std::string name;
    long long num = 0;
    if (!(in >> name >> num)) {
      throw std::invalid_argument("Input error");
    }
    checkInput(in);
    Matrix *matrix = find(storage, name);
    if (!matrix) {
      throw std::runtime_error("Matrix not found");
    }
    size_t r = matrix->getRows();
    size_t c = matrix->getCols();
    for (size_t i = 0; i < r; ++i) {
      for (size_t j = 0; j < c; ++j) {
        Matrix::check((*matrix)[i][j], num, '*');
        (*matrix)[i][j] *= num;
      }
    }
  }

  void eye(std::ostream &, std::istream &in, Storage &storage)
  {
    std::string name;
    size_t n = 0;
    if (!(in >> name >> n)) {
      throw std::invalid_argument("Input error");
    }
    checkInput(in);
    if (n == 0) {
      throw std::invalid_argument("Input error");
    }
    khalikov::Matrix result(n, n, 10);
    for (size_t i = 0; i < n; ++i) {
      for (size_t j = 0; j < n; ++j) {
        if (i == j) {
          result[i][j] = 1;
        } else {
          result[i][j] = 0;
        }
      }
    }
    storage.insert(std::make_pair(name, result));
  }

  void rotateR(std::ostream &, std::istream &in, Storage &storage)
  {
    std::string name;
    if (!(in >> name)) {
      throw std::invalid_argument("Input error");
    }
    checkInput(in);
    const Matrix *matrix = find(storage, name);
    if (!matrix) {
      throw std::runtime_error("Matrix not found");
    }
    size_t r = matrix->getRows();
    size_t c = matrix->getCols();
    khalikov::Matrix result(c, r, matrix->getBase());
    for (size_t i = 0; i < r; ++i) {
      for (size_t j = 0; j < c; ++j) {
        result[j][r - 1 - i] = (*matrix)[i][j];
      }
    }
    std::pair< std::string, Matrix > target(name, *matrix);
    storage.remove(target);
    storage.insert(std::make_pair(name, result));
  }

  void rotateL(std::ostream &, std::istream &in, Storage &storage)
  {
    std::string name;
    if (!(in >> name)) {
      throw std::invalid_argument("Input error");
    }
    checkInput(in);
    const Matrix *matrix = find(storage, name);
    if (!matrix) {
      throw std::runtime_error("Matrix not found");
    }
    size_t r = matrix->getRows();
    size_t c = matrix->getCols();
    khalikov::Matrix result(c, r, matrix->getBase());
    for (size_t i = 0; i < r; ++i) {
      for (size_t j = 0; j < c; ++j) {
        result[c - 1 - j][i] = (*matrix)[i][j];
      }
    }
    std::pair< std::string, Matrix > target(name, *matrix);
    storage.remove(target);
    storage.insert(std::make_pair(name, result));
  }

  void square(std::ostream &, std::istream &in, Storage &storage)
  {
    std::string name;
    if (!(in >> name)) {
      throw std::invalid_argument("Input error");
    }
    checkInput(in);
    const Matrix *matrix = find(storage, name);
    if (!matrix) {
      throw std::runtime_error("Matrix not found");
    }
    size_t r = matrix->getRows();
    size_t c = matrix->getCols();
    size_t k = std::max(r, c);
    khalikov::Matrix result(k, k, matrix->getBase());
    for (size_t i = 0; i < k; ++i) {
      for (size_t j = 0; j < k; ++j) {
        if (i < r && j < c) {
          result[i][j] = (*matrix)[i][j];
        } else {
          result[i][j] = 0;
        }
      }
    }
    std::pair< std::string, Matrix > target(name, *matrix);
    storage.remove(target);
    storage.insert(std::make_pair(name, std::move(result)));
  }

  void mergeH(std::ostream &, std::istream &in, Storage &storage)
  {
    std::string res, arg1, arg2;
    if (!(in >> res >> arg1 >> arg2)) {
      throw std::invalid_argument("Input error");
    }
    checkInput(in);
    const Matrix *m1 = find(storage, arg1);
    const Matrix *m2 = find(storage, arg2);
    if (!m1 || !m2) {
      throw std::runtime_error("Matrix not found");
    }
    if (m1->getRows() != m2->getRows()) {
      throw std::invalid_argument("Impossible to do");
    }
    size_t rows = m1->getRows();
    size_t cols1 = m1->getCols();
    size_t cols2 = m2->getCols();
    khalikov::Matrix result(rows, cols1 + cols2, 10);
    for (size_t i = 0; i < rows; ++i) {
      for (size_t j = 0; j < cols1 + cols2; ++j) {
        if (j < cols1) {
          result[i][j] = (*m1)[i][j];
        } else {
          result[i][j] = (*m2)[i][j - cols1];
        }
      }
    }
    storage.insert(std::make_pair(res, std::move(result)));
  }

  void mergeV(std::ostream &, std::istream &in, Storage &storage)
  {
    std::string res, arg1, arg2;
    if (!(in >> res >> arg1 >> arg2)) {
      throw std::invalid_argument("Input error");
    }
    checkInput(in);
    const Matrix *m1 = find(storage, arg1);
    const Matrix *m2 = find(storage, arg2);
    if (!m1 || !m2) {
      throw std::runtime_error("Matrix not found");
    }
    if (m1->getCols() != m2->getCols()) {
      throw std::invalid_argument("Impossible to do");
    }
    size_t cols = m1->getCols();
    size_t rows1 = m1->getRows();
    size_t rows2 = m2->getRows();
    khalikov::Matrix result(rows1 + rows2, cols, 10);
    for (size_t i = 0; i < rows1 + rows2; ++i) {
      for (size_t j = 0; j < cols; ++j) {
        if (i < rows1) {
          result[i][j] = (*m1)[i][j];
        } else {
          result[i][j] = (*m2)[i - rows1][j];
        }
      }
    }
    storage.insert(std::make_pair(res, std::move(result)));
  }

  void swapR(std::ostream &, std::istream &in, Storage &storage)
  {
    std::string name;
    size_t r1 = 0, r2 = 0;
    if (!(in >> name >> r1 >> r2)) {
      throw std::invalid_argument("Input error");
    }
    checkInput(in);
    Matrix *matrix = find(storage, name);
    if (!matrix) {
      throw std::runtime_error("Matrix not found");
    }
    size_t rows = matrix->getRows();
    if (r1 >= rows || r2 >= rows) {
      throw std::invalid_argument("Impossible to do");
    }
    for (size_t j = 0; j < matrix->getCols(); ++j) {
      std::swap((*matrix)[r1][j], (*matrix)[r2][j]);
    }
  }

  void swapC(std::ostream &, std::istream &in, Storage &storage)
  {
    std::string name;
    size_t c1 = 0, c2 = 0;
    if (!(in >> name >> c1 >> c2)) {
      throw std::invalid_argument("Input error");
    }
    checkInput(in);
    Matrix *matrix = find(storage, name);
    if (!matrix) {
      throw std::runtime_error("Matrix not found");
    }
    size_t cols = matrix->getCols();
    if (c1 >= cols || c2 >= cols) {
      throw std::invalid_argument("Impossible to do");
    }
    for (size_t i = 0; i < matrix->getRows(); ++i) {
      std::swap((*matrix)[i][c1], (*matrix)[i][c2]);
    }
  }

  void reshape(std::ostream &, std::istream &in, Storage &storage)
  {
    std::string name;
    size_t newRows = 0, newCols = 0;
    if (!(in >> name >> newRows >> newCols)) {
      throw std::invalid_argument("Input error");
    }
    checkInput(in);
    Matrix *matrix = find(storage, name);
    if (!matrix) {
      throw std::runtime_error("Matrix not found");
    }
    size_t oldRows = matrix->getRows();
    size_t oldCols = matrix->getCols();
    if (oldRows * oldCols != newRows * newCols) {
      throw std::invalid_argument("Impossible to do");
    }
    khalikov::Matrix result(newRows, newCols, matrix->getBase());
    for (size_t k = 0; k < oldRows * oldCols; ++k) {
      size_t oldR = k / oldCols;
      size_t oldC = k % oldCols;
      size_t newR = k / newCols;
      size_t newC = k % newCols;
      result[newR][newC] = (*matrix)[oldR][oldC];
    }
    std::pair< std::string, Matrix > target(name, *matrix);
    storage.remove(target);
    storage.insert(std::make_pair(name, std::move(result)));
  }

  void splitH(std::ostream &, std::istream &in, Storage &storage)
  {
    std::string res1, res2, name;
    size_t colIdx = 0;
    if (!(in >> res1 >> res2 >> name >> colIdx)) {
      throw std::invalid_argument("Input error");
    }
    checkInput(in);
    const Matrix *matrix = find(storage, name);
    if (!matrix) {
      throw std::runtime_error("Matrix not found");
    }
    size_t rows = matrix->getRows();
    size_t cols = matrix->getCols();
    if (colIdx == 0 || colIdx >= cols) {
      throw std::invalid_argument("Impossible to do");
    }
    khalikov::Matrix m1(rows, colIdx, matrix->getBase());
    khalikov::Matrix m2(rows, cols - colIdx, matrix->getBase());
    for (size_t i = 0; i < rows; ++i) {
      for (size_t j = 0; j < cols; ++j) {
        if (j < colIdx) {
          m1[i][j] = (*matrix)[i][j];
        } else {
          m2[i][j - colIdx] = (*matrix)[i][j];
        }
      }
    }
    storage.insert(std::make_pair(res1, std::move(m1)));
    storage.insert(std::make_pair(res2, std::move(m2)));
  }

  void splitV(std::ostream &, std::istream &in, Storage &storage)
  {
    std::string res1, res2, name;
    size_t rowIdx = 0;
    if (!(in >> res1 >> res2 >> name >> rowIdx)) {
      throw std::invalid_argument("Input error");
    }
    checkInput(in);
    const Matrix *matrix = find(storage, name);
    if (!matrix) {
      throw std::runtime_error("Matrix not found");
    }
    size_t rows = matrix->getRows();
    size_t cols = matrix->getCols();
    if (rowIdx == 0 || rowIdx >= rows) {
      throw std::invalid_argument("Impossible to do");
    }
    khalikov::Matrix m1(rowIdx, cols, matrix->getBase());
    khalikov::Matrix m2(rows - rowIdx, cols, matrix->getBase());
    for (size_t i = 0; i < rows; ++i) {
      for (size_t j = 0; j < cols; ++j) {
        if (i < rowIdx) {
          m1[i][j] = (*matrix)[i][j];
        } else {
          m2[i - rowIdx][j] = (*matrix)[i][j];
        }
      }
    }
    storage.insert(std::make_pair(res1, std::move(m1)));
    storage.insert(std::make_pair(res2, std::move(m2)));
  }

}
