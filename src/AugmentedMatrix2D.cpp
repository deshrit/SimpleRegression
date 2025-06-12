#include "AugmentedMatrix2D.h"

// Constructor
AugmentedMatrix2D::AugmentedMatrix2D(size_t rowSize, size_t colSize, double *m,
                                     double *sol, unsigned int *status) {
  this->rowSize = rowSize;
  this->colSize = colSize;

  solutionStatus = status;

  // Matrix
  am = m;

  // Final solution
  solution = sol;

  // Array storing the order of pivot operations
  pivots = new size_t[rowSize];
  for (size_t i = 0; i < rowSize; i++)
    pivots[i] = i;
}

// Destructor
AugmentedMatrix2D::~AugmentedMatrix2D() { delete[] pivots; }

size_t AugmentedMatrix2D::getIndex(size_t row, size_t col) const {
  return row * colSize + col;
}

double AugmentedMatrix2D::getElement(size_t row, size_t col) const {
  return am[getIndex(row, col)];
}

void AugmentedMatrix2D::setElement(size_t row, size_t col, double val) {
  am[getIndex(row, col)] = val;
}

void AugmentedMatrix2D::swapRow(size_t row1, size_t row2) {
  double tmp;
  for (size_t i = 0; i < colSize; i++) {
    tmp = getElement(row1, i);
    setElement(row1, i, getElement(row2, i));
    setElement(row2, i, tmp);
  }
}

void AugmentedMatrix2D::swapCol(size_t col1, size_t col2) {
  // Update pivot order
  pivots[col1] = col2;
  pivots[col2] = col1;

  double tmp;
  for (size_t i = 0; i < rowSize; i++) {
    tmp = getElement(i, col1);
    setElement(i, col1, getElement(i, col2));
    setElement(i, col2, tmp);
  }
}

void AugmentedMatrix2D::completePivot(size_t startDiagIdx) {
  double tmp = 0.0;
  size_t maxValIdx = startDiagIdx;

  /*
  Iterate vertical from start row and swap row
  with max value with start row
  */
  for (size_t i = startDiagIdx; i < rowSize; i++) {
    if (getElement(i, startDiagIdx) > tmp) {
      tmp = getElement(i, startDiagIdx);
      maxValIdx = i;
    }
  }
  if (maxValIdx != startDiagIdx)
    swapRow(startDiagIdx, maxValIdx);

  tmp = 0.0;
  maxValIdx = startDiagIdx;

  /*
  Iterate horizontal from start col and swap col
  with max value with start col
  */
  for (size_t i = startDiagIdx; i < colSize - 2; i++) {
    if (getElement(startDiagIdx, i) > tmp) {
      tmp = getElement(startDiagIdx, i);
      maxValIdx = i;
    }
  }
  if (maxValIdx != startDiagIdx)
    swapCol(startDiagIdx, maxValIdx);
}

void AugmentedMatrix2D::updateRowCorrectionFactor(size_t refRow,
                                                  size_t currrentRow,
                                                  double cf) {
  for (size_t i = refRow; i < colSize; i++) {
    setElement(currrentRow, i,
               getElement(refRow, i) - cf * getElement(currrentRow, i));
  }
}

bool AugmentedMatrix2D::isRowEchelon() const {
  double sum = 0.0;
  for (size_t i = 1; i < rowSize; i++) {
    for (size_t j = 0; j < i; j++)
      sum += getElement(i, j);
  }
  return (sum == 0.0);
}

void AugmentedMatrix2D::rowEchelonReduction() {
  if (isRowEchelon())
    return;
  double cf = 0.0; // correction factor
  for (size_t i = 0; i < rowSize - 1; i++) {
    for (size_t j = i + 1; j < rowSize; j++) {
      if (!(i == rowSize - 2))
        completePivot(i);
      if (getElement(j, i) == 0.0)
        continue;
      cf = getElement(i, i) / getElement(j, i);
      updateRowCorrectionFactor(i, j, cf);
    }
  }
}

size_t AugmentedMatrix2D::getMatrixRank() const {
  size_t r = 0;
  for (size_t i = 0; i < rowSize; i++) {
    double sum = 0.0;
    for (size_t j = 0; j < colSize - 1; j++) {
      sum += getElement(i, j);
    }
    if (sum != 0.0)
      r++;
  }
  return r;
}

size_t AugmentedMatrix2D::getAugmentedMatrixRank() const {
  size_t r = 0;
  for (size_t i = 0; i < rowSize; i++) {
    double sum = 0.0;
    for (size_t j = 0; j < colSize; j++) {
      sum += getElement(i, j);
    }
    if (sum != 0.0)
      r++;
  }
  return r;
}

void AugmentedMatrix2D::backSubstitution() {
  for (int i = rowSize - 1; i >= 0; i--) {
    double sum = 0.0;
    for (size_t j = i + 1; j < rowSize; j++) {
      sum += solution[pivots[j]] * getElement(i, j);
    }
    solution[pivots[i]] = (getElement(i, colSize - 1) - sum) / getElement(i, i);
  }
}

void AugmentedMatrix2D::solve() {
  size_t rank, augRank;
  rowEchelonReduction();
  rank = getMatrixRank();
  augRank = getAugmentedMatrixRank();
  if (rank == augRank && rank != 0) {
    if (rank == rowSize)
      *solutionStatus = UNIQUE_SOLUTION;
    else
      *solutionStatus = INFINITE_SOLUTIONS;
  } else
    *solutionStatus = NO_SOLUTION;

  if (*solutionStatus == UNIQUE_SOLUTION)
    backSubstitution();
}

// void AugmentedMatrix2D::printMatrix() const {
//     for(size_t i=0; i < this->rowSize; i++) {
//         Serial.print("\n");
//         for(size_t j=0; j < this->colSize; j++) {
//             Serial.print(this->getElement(i, j));
//             Serial.print("\t");
//         }
//         Serial.print("\n");
//     }
// }
