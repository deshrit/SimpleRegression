#pragma once

#include <Arduino.h>

#define UNIQUE_SOLUTION 10
#define INFINITE_SOLUTIONS 20
#define NO_SOLUTION 30

class AugmentedMatrix2D {
private:
  size_t rowSize, colSize, *pivots;
  double *am, *solution;
  unsigned int *solutionStatus;

  size_t getIndex(size_t row, size_t col) const;

  double getElement(size_t row, size_t col) const;
  void setElement(size_t row, size_t col, double val);

  void swapRow(size_t row1, size_t row2);
  void swapCol(size_t col1, size_t col2);

  void completePivot(size_t startDiagIdx);

  void updateRowCorrectionFactor(size_t refRow, size_t currrentRow, double cf);

  bool isRowEchelon() const;

  void rowEchelonReduction();

  unsigned int getMatrixRank() const;

  unsigned int getAugmentedMatrixRank() const;

  void backSubstitution();

public:
  AugmentedMatrix2D(const AugmentedMatrix2D &other) = delete;
  AugmentedMatrix2D(size_t rowSize, size_t colSize, double *m, double *s,
                    unsigned int *status);
  ~AugmentedMatrix2D();

  void solve();
};
