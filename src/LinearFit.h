#pragma once

#include "AugmentedMatrix2D.h"

template <typename T, typename U> class LinearFit {
private:
  double *m, *sol;

public:
  const uint8_t MATRIX_ROW_SIZE = 2, MATRIX_COL_SIZE = 3;
  unsigned int status;

  // Constructor
  LinearFit(const LinearFit &other) = delete;
  LinearFit();
  LinearFit(const T x[], const U y[], const size_t size);

  // Destructor
  ~LinearFit();

  // Fit
  void fit(const T x[], const U y[], const size_t size);

  // Coefficients
  double slope() const;
  double intercept() const;

  // Prediction
  double predict(T x) const;
  void predictMany(const T xTest[], double yPred[], const size_t size) const;

  // Evaluation
  double MAE(const U yTest[], const double yPred[],
             const size_t size) const; // Mean Absolute Error
  double R2Score(const U yTest[], const double yPred[],
                 const size_t size) const; // Coefficient of Determination
};

// Constructor
template <typename T, typename U> LinearFit<T, U>::LinearFit() {
  m = new double[MATRIX_ROW_SIZE * MATRIX_COL_SIZE]{0.0};
  sol = new double[MATRIX_ROW_SIZE];
  status = 0;
}

template <typename T, typename U>
LinearFit<T, U>::LinearFit(const T x[], const U y[], const size_t size) {
  /*
  Suppose system of equation:
  a n  + b ∑x = ∑y
  a ∑x + b ∑x^2 = ∑xy

  Then, all coefficient is represented in augmented matrix form as:
  m = [n, ∑x, ∑y, ∑x, ∑x^2, ∑xy]
  */
  status = 0;
  m = new double[MATRIX_ROW_SIZE * MATRIX_COL_SIZE]{0.0};
  m[0] = size;
  for (size_t i = 0; i < size; i++) {
    m[1] += x[i];
    m[2] += y[i];
    m[3] = m[1];
    m[4] += x[i] * x[i];
    m[5] += x[i] * y[i];
  }
  sol = new double[MATRIX_ROW_SIZE];

  AugmentedMatrix2D am(MATRIX_ROW_SIZE, MATRIX_COL_SIZE, m, sol, &status);
  am.solve();
}

// Destructor
template <typename T, typename U> LinearFit<T, U>::~LinearFit() {
  delete[] m;
  delete[] sol;
}

// Fit
template <typename T, typename U>
void LinearFit<T, U>::fit(const T x[], const U y[], const size_t size) {
  /*
  Suppose system of equation:
  a n  + b ∑x = ∑y
  a ∑x + b ∑x^2 = ∑xy

  Then, all coefficient is represented in augmented matrix form as:
  m = [n, ∑x, ∑y, ∑x, ∑x^2, ∑xy]
  */
  m[0] = size;
  for (size_t i = 0; i < size; i++) {
    m[1] += x[i];
    m[2] += y[i];
    m[3] = m[1];
    m[4] += x[i] * x[i];
    m[5] += x[i] * y[i];
  }

  AugmentedMatrix2D am(MATRIX_ROW_SIZE, MATRIX_COL_SIZE, m, sol, &status);
  am.solve();
}

// Coefficients
template <typename T, typename U> double LinearFit<T, U>::slope() const {
  return sol[1];
}

template <typename T, typename U> double LinearFit<T, U>::intercept() const {
  return sol[0];
}

// Prediction
template <typename T, typename U> double LinearFit<T, U>::predict(T x) const {
  return sol[0] + sol[1] * x;
}

template <typename T, typename U>
void LinearFit<T, U>::predictMany(const T xTest[], double yPred[],
                                  const size_t size) const {
  for (size_t i = 0; i < size; i++)
    yPred[i] = predict(xTest[i]);
}

// Evaluation
template <typename T, typename U>
double LinearFit<T, U>::MAE(const U yTest[], const double yPred[],
                            const size_t size) const {
  double sum = 0.0;
  for (size_t i = 0; i < size; i++) {
    sum += fabs(yTest[i] - yPred[i]);
  }
  return sum / size;
}

template <typename T, typename U>
double LinearFit<T, U>::R2Score(const U yTest[], const double yPred[],
                                const size_t size) const {
  double yBar, R2, sum = 0.0, sum2 = 0.0;
  for (size_t i = 0; i < size; i++) {
    sum += yTest[i];
  }
  yBar = sum / size;

  sum = 0.0;
  for (size_t i = 0; i < size; i++) {
    sum += sq(yTest[i] - yPred[i]);
    sum2 += sq(yTest[i] - yBar);
  }
  R2 = 1 - (sum / sum2);
  return R2;
}
