#ifndef SRC_S21_MATRIX_OOP_
#define SRC_S21_MATRIX_OOP_

#include <iostream>

class S21Matrix {
 private:
  int rows_, cols_;
  double** matrix_;

 public:
  S21Matrix();
  S21Matrix(int rows, int cols);
  S21Matrix(const S21Matrix& other);
  S21Matrix(S21Matrix&& other);
  ~S21Matrix();

  bool EqMatrix(const S21Matrix& other);
  void SumMatrix(const S21Matrix& other);
  void SubMatrix(const S21Matrix& other);
  void MulNumber(const double num);
  void MulMatrix(const S21Matrix& other);
  S21Matrix Transpose();
  S21Matrix CalcComplements();
  double Determinant();
  S21Matrix InverseMatrix();

  S21Matrix operator+(S21Matrix& other);
  S21Matrix operator-(S21Matrix& other);
  S21Matrix operator*(S21Matrix& other);
  S21Matrix operator*(double num);
  bool operator==(S21Matrix& other);
  bool operator!=(S21Matrix& other);
  S21Matrix& operator=(S21Matrix& other);
  S21Matrix& operator=(S21Matrix&& other);
  S21Matrix& operator+=(const S21Matrix& other);
  S21Matrix& operator-=(const S21Matrix& other);
  S21Matrix& operator*=(const S21Matrix& other);
  S21Matrix& operator*=(double num);
  double& operator()(int row, int col);
  const double& operator()(int row, int col) const;

  int GetCols();
  int GetRows();
  void SetCols(int cols);
  void SetRows(int rows);
  void SetSize(int rows, int cols);
  double DeterminantHelper();
  S21Matrix GetMinor(int m_row, int m_col);
  int MatrixPow(int value);
  double Fabs(double value);
  void MemoryAllocation();
  void FreeMemory();
};

#endif  // SRC_S21_MATRIX_OOP_
