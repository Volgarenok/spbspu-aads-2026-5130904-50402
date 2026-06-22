#include "Actions.hpp"
#include <limits>

namespace novikov
{
  void Create(std::istream &in, std::ostream &out, MatrixHash &db)
  {
    std::string name;
    size_t rows, cols;
    if (!(in >> name >> rows >> cols)) {
      throw std::logic_error("Invalid command");
    }
    if (db.has(name)) {
      throw std::logic_error("Matrix already exists");
    }
    Matrix *m = new Matrix(rows, cols);
    db.insert(name, m);
  }

  void Show(std::istream &in, std::ostream &out, MatrixHash &db)
  {
    std::string name;
    if (!(in >> name)) {
      throw std::logic_error("Invalid command");
    }
    Matrix *m = db.get(name);
    m->print();
  }

  void Delete(std::istream &in, std::ostream &out, MatrixHash &db)
  {
    std::string name;
    if (!(in >> name)) {
      throw std::logic_error("Invalid command");
    }
    Matrix *m = db.get(name);
    delete m;
    db.remove(name);
  }

  void Change(std::istream &in, std::ostream &out, MatrixHash &db)
  {
    std::string name;
    size_t i, j;
    double val;
    if (!(in >> name >> i >> j >> val)) {
      throw std::logic_error("Invalid command");
    }
    Matrix *m = db.get(name);
    m->change(i, j, val);
  }

  void AddRow(std::istream &in, std::ostream &out, MatrixHash &db)
  {
    std::string name;
    size_t index;
    if (!(in >> name >> index)) {
      throw std::logic_error("Invalid command");
    }
    Matrix *m = db.get(name);
    size_t cols = m->getCols();
    double *values = new double[cols];
    size_t count = 0;
    double val;
    while (count < cols && (in >> val)) {
      values[count++] = val;
    }
    if (count != cols) {
      delete[] values;
      throw std::logic_error("Invalid command");
    }
    m->addRow(index, values, cols);
    delete[] values;
  }

  void AddCol(std::istream &in, std::ostream &out, MatrixHash &db)
  {
    std::string name;
    size_t index;
    if (!(in >> name >> index)) {
      throw std::logic_error("Invalid command");
    }
    Matrix *m = db.get(name);
    size_t rows = m->getRows();
    double *values = new double[rows];
    size_t count = 0;
    double val;
    while (count < rows && (in >> val)) {
      values[count++] = val;
    }
    if (count != rows) {
      delete[] values;
      throw std::logic_error("Invalid command");
    }
    m->addCol(index, values, rows);
    delete[] values;
  }

  void SwapRows(std::istream &in, std::ostream &out, MatrixHash &db)
  {
    std::string name;
    size_t i1, i2;
    if (!(in >> name >> i1 >> i2)) {
      throw std::logic_error("Invalid command");
    }
    Matrix *m = db.get(name);
    m->swapRows(i1, i2);
  }

  void SwapCols(std::istream &in, std::ostream &out, MatrixHash &db)
  {
    std::string name;
    size_t j1, j2;
    if (!(in >> name >> j1 >> j2)) {
      throw std::logic_error("Invalid command");
    }
    Matrix *m = db.get(name);
    m->swapCols(j1, j2);
  }

  void DeleteRow(std::istream &in, std::ostream &out, MatrixHash &db)
  {
    std::string name;
    size_t index;
    if (!(in >> name >> index)) {
      throw std::logic_error("Invalid command");
    }
    Matrix *m = db.get(name);
    m->deleteRow(index);
  }

  void DeleteCol(std::istream &in, std::ostream &out, MatrixHash &db)
  {
    std::string name;
    size_t index;
    if (!(in >> name >> index)) {
      throw std::logic_error("Invalid command");
    }
    Matrix *m = db.get(name);
    m->deleteCol(index);
  }

  void Sum(std::istream &in, std::ostream &out, MatrixHash &db)
  {
    std::string newName, name1, name2;
    if (!(in >> newName >> name1 >> name2)) {
      throw std::logic_error("Invalid command");
    }
    if (db.has(newName)) {
      throw std::logic_error("Invalid command");
    }
    Matrix *m1 = db.get(name1);
    Matrix *m2 = db.get(name2);
    Matrix result = m1->add(*m2);
    Matrix *newMatrix = new Matrix(result);
    db.insert(newName, newMatrix);
  }

  void Sub(std::istream &in, std::ostream &out, MatrixHash &db)
  {
    std::string newName, name1, name2;
    if (!(in >> newName >> name1 >> name2)) {
      throw std::logic_error("Invalid command");
    }
    if (db.has(newName)) {
      throw std::logic_error("Invalid command");
    }
    Matrix *m1 = db.get(name1);
    Matrix *m2 = db.get(name2);
    Matrix result = m1->subtract(*m2);
    Matrix *newMatrix = new Matrix(result);
    db.insert(newName, newMatrix);
  }

  void Mul(std::istream &in, std::ostream &out, MatrixHash &db)
  {
    std::string newName, name1, name2;
    if (!(in >> newName >> name1 >> name2)) {
      throw std::logic_error("Invalid command");
    }
    if (db.has(newName)) {
      throw std::logic_error("Invalid command");
    }
    Matrix *m1 = db.get(name1);
    Matrix *m2 = db.get(name2);
    Matrix result = m1->multiply(*m2);
    Matrix *newMatrix = new Matrix(result);
    db.insert(newName, newMatrix);
  }

  void ScalarMul(std::istream &in, std::ostream &out, MatrixHash &db)
  {
    std::string name;
    double scalar;
    if (!(in >> name >> scalar)) {
      throw std::logic_error("Invalid command");
    }
    Matrix *m = db.get(name);
    Matrix result = m->scalarMultiply(scalar);
    *m = result;
  }

  void Transpose(std::istream &in, std::ostream &out, MatrixHash &db)
  {
    std::string newName, name;
    if (!(in >> newName >> name)) {
      throw std::logic_error("Invalid command");
    }
    if (db.has(newName)) {
      throw std::logic_error("Invalid command");
    }
    Matrix *m = db.get(name);
    Matrix result = m->transpose();
    Matrix *newMatrix = new Matrix(result);
    db.insert(newName, newMatrix);
  }

  void Det(std::istream &in, std::ostream &out, MatrixHash &db)
  {
    std::string name;
    if (!(in >> name)) {
      throw std::logic_error("Invalid command");
    }
    Matrix *m = db.get(name);
    double det = m->determinant();
    out << det << "\n";
  }

  void Lu(std::istream &in, std::ostream &out, MatrixHash &db)
  {
    std::string name;
    if (!(in >> name)) {
      throw std::logic_error("Invalid command");
    }
    Matrix *m = db.get(name);
    Matrix L, U;
    if (!m->luDecomposition(L, U)) {
      throw std::logic_error("Matrix is singular");
    }
    out << "L:" << "\n";
    L.print();
    out << "U:" << "\n";
    U.print();
  }

  void Solve(std::istream &in, std::ostream &out, MatrixHash &db)
  {
    std::string name;
    if (!(in >> name)) {
      throw std::logic_error("Invalid command");
    }
    Matrix *m = db.get(name);
    size_t rows = m->getRows();
    double *b = new double[rows];
    for (size_t i = 0; i < rows; ++i) {
      if (!(in >> b[i])) {
        delete[] b;
        throw std::logic_error("Invalid command");
      }
    }
    double *x = m->solveSystem(b);
    for (size_t i = 0; i < rows; ++i) {
      out << x[i] << " ";
    }
    out << "\n";
    delete[] b;
    delete[] x;
  }
}
