#include <ParabolicFit.h>

// Dataset1
const size_t TRAIN_SIZE = 52;

int xTrain[TRAIN_SIZE] = {-30, -29, -28, -27, -26, -25, -24, -23, -22, -21, -20,
                          -19, -18, -17, -16, -15, -14, -13, -12, -11, -10, -9,
                          -8,  -7,  -6,  -5,  -4,  -3,  -2,  -1,  0,   1,   2,
                          3,   4,   5,   6,   7,   8,   9,   10,  11,  12,  13,
                          14,  15,  16,  17,  18,  19,  20,  21};

double yTrain[TRAIN_SIZE] = {
    540.07979026, 523.31478226, 450.09047389, 441.58276052, 450.29469543,
    422.12930889, 317.55853105, 299.22428781, 258.14672033, 272.58098437,
    207.98762109, 228.18332244, 225.91440476, 168.34872826, 188.6254902,
    161.58628174, 87.39818824,  124.3854088,  78.92524469,  80.0101197,
    53.35250818,  81.81379183,  45.04537177,  6.18677385,   35.4649018,
    50.53147645,  -4.36364676,  -19.30421161, -8.20287424,  42.13836499,
    42.59954225,  9.73836759,   31.28341085,  25.07508713,  -21.20059587,
    -48.27829911, 10.34109025,  4.28407624,   -10.84352369, -18.99454952,
    5.65199164,   19.39402487,  7.92355783,   19.67927742,  46.76863993,
    98.55339129,  83.62529045,  66.84878624,  80.79775066,  113.96220755,
    105.54103446, 122.6378711};

const size_t TEST_SIZE = 8;

int xTest[TEST_SIZE] = {22, 23, 24, 25, 26, 27, 28, 29};
double yTest[TEST_SIZE] = {172.40350598, 149.22293688, 209.35855324,
                           218.71762124, 275.76866218, 303.17462231,
                           322.10929219, 347.41804138};

void setup() {
  Serial.begin(9600);

  // Model
  ParabolicFit<int, double> pf;
  pf.fit(xTrain, yTrain, TRAIN_SIZE);

  // Coefficients
  Serial.print("\nFor ax^2 + bx + c the coefficients are, a: ");
  Serial.print(pf.a());
  Serial.print(",\tb: ");
  Serial.print(pf.b());
  Serial.print(",\tc: ");
  Serial.print(pf.c());

  // Predict
  Serial.print("\nPrediction of 22: ");
  Serial.print(pf.predict(22));

  // Predict many
  double yPred[TEST_SIZE];
  pf.predictMany(xTest, yPred, TEST_SIZE);

  // Evaluate
  Serial.print("\nMAE(Mean Absolute Error): ");
  Serial.print(pf.MAE(yTest, yPred, TEST_SIZE));
}

void loop() {}
