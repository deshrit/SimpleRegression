#pragma once

#include "AugmentedMatrix2D.h"

template <typename T, typename U> class ParabolicFit {
private:
  double *m, *sol;

public:
  const uint8_t MATRIX_ROW_SIZE = 3, MATRIX_COL_SIZE = 4;
  unsigned int status;

  // Constructor
  ParabolicFit(const ParabolicFit &other) = delete;
  ParabolicFit();
  ParabolicFit(const T x[], const U y[], const size_t size);

  // Destructor
  ~ParabolicFit();

  // Fit
  void fit(const T x[], const U y[], const size_t size);

  // Coefficients
  double a() const;
  double b() const;
  double c() const;

  // Prediction
  double predict(T x) const;
  void predictMany(const T xTest[], double yPred[], const size_t size) const;

  // Evaluation
  double MAE(const U yTest[], const double yPred[],
             const size_t size) const; // Mean Absolute Error
};

// Constructor
template <typename T, typename U> ParabolicFit<T, U>::ParabolicFit() {
  m = new double[MATRIX_ROW_SIZE * MATRIX_COL_SIZE]{0.0};
  sol = new double[MATRIX_ROW_SIZE];
  status = 0;
}

template <typename T, typename U>
ParabolicFit<T, U>::ParabolicFit(const T x[], const U y[], const size_t size) {
  /*
    Suppose system of equation:
    a n    + b ∑x   + c ∑x^2 = ∑y
    a ∑x   + b ∑x^2 + c ∑x^3 = ∑xy
    a ∑x^2 + b ∑x^3 + c ∑x^4 = ∑ (x^2 *y)

    Then, all coefficient is represented in augmented matrix form as:
    m = [n, ∑x, ∑x^2, ∑y, ∑x, ∑x^2, ∑x^3, ∑xy, ∑x^2, ∑x^3, ∑x^4, ∑ (x^2 * y)]
    */
  status = 0;
  m = new double[MATRIX_ROW_SIZE * MATRIX_COL_SIZE]{0.0};
  m[0] = size;
  for (size_t i = 0; i < size; i++) {
    m[1] += x[i];
    m[2] += x[i] * x[i];
    m[3] += y[i];

    m[4] = m[1];
    m[5] = m[2];
    m[6] += x[i] * x[i] * x[i];
    m[7] += x[i] * y[i];

    m[8] = m[5];
    m[9] = m[6];
    m[10] += x[i] * x[i] * x[i] * x[i];
    m[11] += x[i] * x[i] * y[i];
  }
  sol = new double[MATRIX_ROW_SIZE];

  AugmentedMatrix2D am(MATRIX_ROW_SIZE, MATRIX_COL_SIZE, m, sol, &status);
  am.solve();
}

// Destructor
template <typename T, typename U> ParabolicFit<T, U>::~ParabolicFit() {
  delete[] m;
  delete[] sol;
}

// Fit
template <typename T, typename U>
void ParabolicFit<T, U>::fit(const T x[], const U y[], const size_t size) {
  /*
    Suppose system of equation:
    a n    + b ∑x   + c ∑x^2 = ∑y
    a ∑x   + b ∑x^2 + c ∑x^3 = ∑xy
    a ∑x^2 + b ∑x^3 + c ∑x^4 = ∑ (x^2 *y)

    Then, all coefficient is represented in augmented matrix form as:
    m = [n, ∑x, ∑x^2, ∑y, ∑x, ∑x^2, ∑x^3, ∑xy, ∑x^2, ∑x^3, ∑x^4, ∑ (x^2 * y)]
    */
  m[0] = size;
  for (size_t i = 0; i < size; i++) {
    m[1] += x[i];
    m[2] += pow(x[i], 2);
    m[3] += y[i];

    m[4] = m[1];
    m[5] = m[2];
    m[6] += pow(x[i], 3);
    m[7] += x[i] * y[i];

    m[8] = m[5];
    m[9] = m[6];
    m[10] += pow(x[i], 4);
    m[11] += pow(x[i], 2) * y[i];
  }

  AugmentedMatrix2D am(MATRIX_ROW_SIZE, MATRIX_COL_SIZE, m, sol, &status);
  // am.printMatrix();
  am.solve();
}

// Coefficients
template <typename T, typename U> double ParabolicFit<T, U>::a() const {
  return sol[2];
}

template <typename T, typename U> double ParabolicFit<T, U>::b() const {
  return sol[1];
}

template <typename T, typename U> double ParabolicFit<T, U>::c() const {
  return sol[0];
}

// Prediction
template <typename T, typename U>
double ParabolicFit<T, U>::predict(T x) const {
  return sol[0] + sol[1] * x + sol[2] * x * x;
}

template <typename T, typename U>
void ParabolicFit<T, U>::predictMany(const T xTest[], double yPred[],
                                     const size_t size) const {
  for (size_t i = 0; i < size; i++)
    yPred[i] = predict(xTest[i]);
}

// Evaluation
template <typename T, typename U>
double ParabolicFit<T, U>::MAE(const U yTest[], const double yPred[],
                               const size_t size) const {
  double sum = 0.0;
  for (size_t i = 0; i < size; i++) {
    sum += fabs(yTest[i] - yPred[i]);
  }
  return sum / size;
}
