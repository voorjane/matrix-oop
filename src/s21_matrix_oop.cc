#include "s21_matrix_oop.h"

S21Matrix::S21Matrix() {
  rows_ = 0, cols_ = 0;
  matrix_ = nullptr;
}

S21Matrix::S21Matrix(int rows, int cols) : rows_(rows), cols_(cols) {
  if (rows < 0 || cols < 0)
    throw std::invalid_argument("Number of rows or columns should be positive");
  MemoryAllocation();
}

S21Matrix::S21Matrix(const S21Matrix& other)
    : rows_(other.rows_), cols_(other.cols_) {
  MemoryAllocation();
  for (int row = 0; row < rows_; row++) {
    for (int col = 0; col < cols_; col++) {
      (*this)(row, col) = other(row, col);
    }
  }
}

S21Matrix::S21Matrix(S21Matrix&& other)
    : rows_(other.rows_), cols_(other.cols_) {
  matrix_ = std::exchange(other.matrix_, nullptr);
  other.rows_ = 0, other.cols_ = 0;
}

S21Matrix::~S21Matrix() { FreeMemory(); }

bool S21Matrix::EqMatrix(const S21Matrix& other) {
  bool code = true;
  if (cols_ != other.cols_ || rows_ != other.rows_) code = false;
  for (int row = 0; row < rows_ && code; row++) {
    for (int col = 0; col < cols_ && code; col++) {
      if (S21Matrix::Fabs(matrix_[row][col] - other.matrix_[row][col]) > 1e-7)
        code = false;
    }
  }
  return code;
}

void S21Matrix::SumMatrix(const S21Matrix& other) {
  if (rows_ != other.rows_ || cols_ != other.cols_)
    throw std::out_of_range("Different matrix dimensions");
  for (int row = 0; row < rows_; row++) {
    for (int col = 0; col < cols_; col++) {
      matrix_[row][col] += other.matrix_[row][col];
    }
  }
}

void S21Matrix::SubMatrix(const S21Matrix& other) {
  if (rows_ != other.rows_ || cols_ != other.cols_)
    throw std::out_of_range("Different matrix dimensions");
  for (int row = 0; row < rows_; row++) {
    for (int col = 0; col < cols_; col++) {
      matrix_[row][col] -= other.matrix_[row][col];
    }
  }
}

void S21Matrix::MulNumber(const double num) {
  for (int row = 0; row < rows_; row++)
    for (int col = 0; col < cols_; col++) matrix_[row][col] *= num;
}

void S21Matrix::MulMatrix(const S21Matrix& other) {
  if (cols_ != other.rows_)
    throw std::out_of_range(
        "Invalid matrix sizes: number of cols of the first matrix must be "
        "equal to the number of rows of the second matrix");
  S21Matrix result(rows_, other.cols_);
  for (int row = 0; row < rows_; row++) {
    for (int col = 0; col < other.cols_; col++) {
      for (int i = 0; i < cols_; i++) {
        result.matrix_[row][col] += matrix_[row][i] * other.matrix_[i][col];
      }
    }
  }
  FreeMemory();
  *this = std::move(result);
}

S21Matrix S21Matrix::Transpose() {
  S21Matrix result(rows_, cols_);
  for (int row = 0; row < rows_; row++) {
    for (int col = 0; col < cols_; col++) {
      result.matrix_[col][row] = matrix_[row][col];
    }
  }
  return result;
}

S21Matrix S21Matrix::CalcComplements() {
  S21Matrix result(rows_, cols_);
  if (rows_ != cols_ || rows_ <= 1 || cols_ <= 1)
    throw std::invalid_argument("The matrix is not square");
  for (int row = 0; row < rows_; row++) {
    for (int col = 0; col < cols_; col++) {
      S21Matrix minor(rows_ - 1, cols_ - 1);
      minor = GetMinor(row, col);
      double det = minor.Determinant();
      result.matrix_[row][col] = det * MatrixPow(row + col);
    }
  }
  return result;
}

double S21Matrix::Determinant() {
  if (rows_ != cols_) throw std::invalid_argument("The matrix is not square");
  return DeterminantHelper();
}

S21Matrix S21Matrix::InverseMatrix() {
  double det = Determinant();
  if (det == 0 || (rows_ != cols_))
    throw std::invalid_argument(
        "Matrix determinant is 0 or matrix is not square");
  S21Matrix result(rows_, cols_);
  if (rows_ == 1 && cols_ == 1) {
    result.matrix_[0][0] = 1 / matrix_[0][0];
    return result;
  }
  result = CalcComplements();
  result = result.Transpose();
  result.MulNumber(1 / det);
  return result;
}

S21Matrix S21Matrix::operator+(S21Matrix& other) {
  S21Matrix result(*this);
  result.SumMatrix(other);
  return result;
}

S21Matrix S21Matrix::operator-(S21Matrix& other) {
  S21Matrix result(*this);
  result.SubMatrix(other);
  return result;
}

S21Matrix S21Matrix::operator*(S21Matrix& other) {
  S21Matrix result(*this);
  result.MulMatrix(other);
  return result;
}

S21Matrix S21Matrix::operator*(double num) {
  S21Matrix result(*this);
  result.MulNumber(num);
  return result;
}

bool S21Matrix::operator==(S21Matrix& other) { return EqMatrix(other); }

bool S21Matrix::operator!=(S21Matrix& other) { return !EqMatrix(other); }

S21Matrix& S21Matrix::operator=(S21Matrix& other) {
  if (this != &other) {
    FreeMemory();
    rows_ = other.rows_;
    cols_ = other.cols_;
    MemoryAllocation();
    for (int i = 0; i < rows_; i++) {
      for (int j = 0; j < cols_; j++) {
        (*this)(i, j) = other(i, j);
      }
    }
  }
  return *this;
}

S21Matrix& S21Matrix::operator=(S21Matrix&& other) {
  if (this != &other) {
    FreeMemory();
    rows_ = std::exchange(other.rows_, 0);
    cols_ = std::exchange(other.cols_, 0);
    matrix_ = std::exchange(other.matrix_, nullptr);
  }
  return *this;
}

S21Matrix& S21Matrix::operator+=(const S21Matrix& other) {
  SumMatrix(other);
  return *this;
}

S21Matrix& S21Matrix::operator-=(const S21Matrix& other) {
  SubMatrix(other);
  return *this;
}

S21Matrix& S21Matrix::operator*=(const S21Matrix& other) {
  MulMatrix(other);
  return *this;
}

S21Matrix& S21Matrix::operator*=(double num) {
  MulNumber(num);
  return *this;
}

double& S21Matrix::operator()(int row, int col) {
  if (row >= rows_ || col >= cols_) {
    throw std::out_of_range("Incorrect input, index is out of range");
  }
  return matrix_[row][col];
}

const double& S21Matrix::operator()(int row, int col) const {
  if (row >= rows_ || col >= cols_) {
    throw std::out_of_range("Incorrect input, index is out of range");
  }
  return matrix_[row][col];
}

int S21Matrix::GetCols() { return cols_; }

int S21Matrix::GetRows() { return rows_; }

void S21Matrix::SetCols(int cols) { SetSize(rows_, cols); }

void S21Matrix::SetRows(int rows) { SetSize(rows, cols_); }

void S21Matrix::SetSize(int rows, int cols) {
  if (rows <= 0 || cols <= 0)
    throw std::invalid_argument("Incorrect input, need rows, cols > 0");
  S21Matrix temp(*this);
  FreeMemory();
  rows_ = rows;
  cols_ = cols;
  MemoryAllocation();
  for (int i = 0; i < std::min(rows_, temp.rows_); i++) {
    for (int j = 0; j < std::min(cols_, temp.cols_); j++) {
      matrix_[i][j] = temp.matrix_[i][j];
    }
  }
}

double S21Matrix::DeterminantHelper() {
  if (rows_ == 0) return 1;
  if (rows_ == 1) return matrix_[0][0];
  if (rows_ == 2)
    return matrix_[0][0] * matrix_[1][1] - matrix_[1][0] * matrix_[0][1];
  double result = 0.0;
  for (int col = 0; col < cols_; col++) {
    S21Matrix minor(rows_ - 1, cols_ - 1);
    double minor_det;
    minor = GetMinor(0, col);
    minor_det = minor.Determinant();
    result += MatrixPow(col) * matrix_[0][col] * minor_det;
  }
  return result;
}

S21Matrix S21Matrix::GetMinor(int m_row, int m_col) {
  int i = 0, j = 0;
  S21Matrix result(rows_ - 1, cols_ - 1);
  for (int row = 0; row < rows_; row++) {
    for (int col = 0; col < cols_; col++) {
      if (row != m_row && col != m_col) {
        result.matrix_[i][j++] = matrix_[row][col];
        if (j == rows_ - 1) {
          j = 0;
          i++;
        }
      }
    }
  }
  return result;
}

int S21Matrix::MatrixPow(int value) { return value % 2 == 0 ? 1 : -1; }

double S21Matrix::Fabs(double value) { return value < 0 ? -value : value; }

void S21Matrix::MemoryAllocation() {
  matrix_ = new double*[rows_];
  for (int row = 0; row < rows_; row++) matrix_[row] = new double[cols_];
}

void S21Matrix::FreeMemory() {
  if (matrix_) {
    for (int row = 0; row < rows_; row++) delete[] matrix_[row];
    delete matrix_;
  }
  rows_ = 0, cols_ = 0;
  matrix_ = nullptr;
}
