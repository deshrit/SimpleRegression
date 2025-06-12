#include <LinearFit.h>

const size_t TRAIN_SIZE = 52;

int xTrain[TRAIN_SIZE] = {-30, -29, -28, -27, -26, -25, -24, -23, -22, -21, -20,
                          -19, -18, -17, -16, -15, -14, -13, -12, -11, -10, -9,
                          -8,  -7,  -6,  -5,  -4,  -3,  -2,  -1,  0,   1,   2,
                          3,   4,   5,   6,   7,   8,   9,   10,  11,  12,  13,
                          14,  15,  16,  17,  18,  19,  20,  21};

double yTrain[TRAIN_SIZE] = {
    -89.9680839,  -80.6740871,  -94.36381044, -82.56689579, -64.28212183,
    -61.14827644, -88.97658758, -82.71028487, -85.94131187, -67.36760625,
    -80.80495156, -60.72667102, -50.0342381,  -61.86050869, -42.94980392,
    -43.3654873,  -63.0407247,  -38.64583648, -47.62990212, -38.39595212,
    -40.65899673, -21.27448327, -28.38185129, -36.72529046, -18.21403928,
    -5.78740942,  -21.7454587,  -22.12168464, -12.4811497,  12.45534599,
    17.0398169,   7.89534704,   20.11336434,  20.83003485,  5.11976165,
    -3.31131965,  22.1364361,   21.3136305,   16.46259052,  14.00218019,
    24.26079666,  29.75760995,  24.76942313,  28.67171097,  38.30745597,
    57.42135652,  49.45011618,  40.3395145,   43.11910026,  53.18488302,
    46.21641378,  49.05514844};

const size_t TEST_SIZE = 8;

int xTest[TEST_SIZE] = {22, 23, 24, 25, 26, 27, 28, 29};

double yTest[TEST_SIZE] = {64.56140239, 50.48917475, 69.34342129, 67.4870485,
                           84.30746487, 88.86984892, 89.64371687, 92.56721655};

void setup() {
  Serial.begin(9600);

  // Model
  LinearFit<int, double> lf;
  lf.fit(xTrain, yTrain, TRAIN_SIZE);

  // Coefficients
  Serial.print("Slope: ");
  Serial.print(lf.slope());
  Serial.print("\nIntercept: ");
  Serial.print(lf.intercept());

  // Predict
  Serial.print("\nPrediction of 22: ");
  Serial.print(lf.predict(22));

  // Predict many
  double yPred[TEST_SIZE];
  lf.predictMany(xTest, yPred, TEST_SIZE);

  // Evaluate
  Serial.print("\nMAE(Mean Absolute Error): ");
  Serial.print(lf.MAE(yTest, yPred, TEST_SIZE));
  Serial.print("\nR2Score: ");
  Serial.print(lf.R2Score(yTest, yPred, TEST_SIZE));
}

void loop() {}
