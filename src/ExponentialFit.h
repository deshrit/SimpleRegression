#pragma once

#include "AugmentedMatrix2D.h"

template <typename T, typename U> class ExponentialFit {
private:
  double *m, *sol;

public:
  const uint8_t MATRIX_ROW_SIZE = 2, MATRIX_COL_SIZE = 3;
  unsigned int status;

  // Constructor
  ExponentialFit(const ExponentialFit &other) = delete;
  ExponentialFit();
  ExponentialFit(const T x[], const U y[], const size_t size);

  // Destructor
  ~ExponentialFit();

  // Fit
  void fit(const T x[], const U y[], const size_t size);

  // Coefficients
  double a() const;
  double b() const;

  // Prediction
  double predict(T x) const;
  void predictMany(const T xTest[], double yPred[], const size_t size) const;

  // Evaluation
  double MAE(const U yTest[], const double yPred[],
             const size_t size) const; // Mean Absolute Error
};

// Constructor
template <typename T, typename U> ExponentialFit<T, U>::ExponentialFit() {
  m = new double[MATRIX_ROW_SIZE * MATRIX_COL_SIZE]{0.0};
  sol = new double[MATRIX_ROW_SIZE];
  status = 0;
}

template <typename T, typename U>
ExponentialFit<T, U>::ExponentialFit(const T x[], const U y[],
                                     const size_t size) {
  /*
  To fit y = a * (b ^ x), express as:

  or, log(y) = log(a) + xlog(b)
  or, Y = A + Bx

  Such that system of equation is:
  a n  + b ∑X = ∑Y
  a ∑X + b ∑X^2 = ∑XY

  Then, all coefficient is represented in augmented matrix form as:
  m = [n, ∑x, ∑y, ∑x, ∑x^2, ∑xy]
  */
  status = 0;
  m[0] = size;
  for (size_t i = 0; i < size; i++) {
    m[1] += x[i];
    m[2] += log(y[i]);
    m[3] = m[1];
    m[4] += x[i] * x[i];
    m[5] += x[i] * log(y[i]);
  }
  sol = new double[MATRIX_ROW_SIZE];

  AugmentedMatrix2D am(MATRIX_ROW_SIZE, MATRIX_COL_SIZE, m, sol, &status);
  am.solve();

  // Take exponential
  sol[0] = exp(sol[0]);
  sol[1] = exp(sol[1]);
}

// Destructor
template <typename T, typename U> ExponentialFit<T, U>::~ExponentialFit() {
  delete[] m;
  delete[] sol;
}

// Fit
template <typename T, typename U>
void ExponentialFit<T, U>::fit(const T x[], const U y[], const size_t size) {
  /*
  To fit y = a * (b ^ x), express as:

  or, log(y) = log(a) + xlog(b)
  or, Y = A + Bx

  Such that system of equation is:
  a n  + b ∑X = ∑Y
  a ∑X + b ∑X^2 = ∑XY

  Then, all coefficient is represented in augmented matrix form as:
  m = [n, ∑x, ∑y, ∑x, ∑x^2, ∑xy]
  */
  m[0] = size;
  for (size_t i = 0; i < size; i++) {
    m[1] += x[i];
    m[2] += log(y[i]);
    m[3] = m[1];
    m[4] += x[i] * x[i];
    m[5] += x[i] * log(y[i]);
  }

  AugmentedMatrix2D am(MATRIX_ROW_SIZE, MATRIX_COL_SIZE, m, sol, &status);
  am.solve();

  // Take exponential
  sol[0] = exp(sol[0]);
  sol[1] = exp(sol[1]);
}

// Coefficients
template <typename T, typename U> double ExponentialFit<T, U>::a() const {
  return sol[0];
}

template <typename T, typename U> double ExponentialFit<T, U>::b() const {
  return sol[1];
}

// Prediction
template <typename T, typename U>
double ExponentialFit<T, U>::predict(T x) const {
  return sol[0] * pow(sol[1], x);
}

template <typename T, typename U>
void ExponentialFit<T, U>::predictMany(const T xTest[], double yPred[],
                                       const size_t size) const {
  for (size_t i = 0; i < size; i++)
    yPred[i] = predict(xTest[i]);
}

// Evaluation
template <typename T, typename U>
double ExponentialFit<T, U>::MAE(const U yTest[], const double yPred[],
                                 const size_t size) const {
  double sum = 0.0;
  for (size_t i = 0; i < size; i++) {
    sum += fabs(yTest[i] - yPred[i]);
  }
  return sum / size;
}
