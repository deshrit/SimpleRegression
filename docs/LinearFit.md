This class provides the functionality for calculating single variable linear
regression.


## Instantiation

Instantiating the class with constructor will automatically fit the data. The template
type should only be the valid numeric data type supported by the arduino framework.

```c++
const size_t size = 50;

double X[size] = {...};

double y[size] = {...};

LinearFit<double, double> lf(X, y, size);
```

## Methods


### 1. fit()

*Syntax*

```c++
LinearFit<double, double> lf;
lf.fit(X, y, size);
```

*Parameters*

- `X`: Numeric array. In above code snippet it is `double[]`.

- `y`: Numeric array. In above code snippet it is `double[]`.

- `size`: Type of `size_t`.

*Returns*

`void`

*Descrption*

Model can be fitted after instantiating the `LinearFit` without constructor or can be re-fitted.



### 2. slope()

*Syntax*

```c++
LinearFit<double, double> lf;
lf.fit(X, y, size);

double s = lf.slope();
```

*Parameters*

`None`

*Returns*

`double`: Slope of the regression line.

*Descrption*

Called after model is fitted, it returns the slope of fitted regression line.



### 3. intercept()

*Syntax*

```c++
LinearFit<double, double> lf;
lf.fit(X, y, size);

double s = lf.intercept();
```

*Parameters*

`None`

*Returns*

`double`: Intercept of the regression line.

*Descrption*

Called after model is fitted, it returns the intercept of fitted regression line.




### 4. predict()

*Syntax*

```c++
LinearFit<int, float> lf;
lf.fit(X, y, size);

int x = 2;
double result = lf.predict(x);
```

*Parameters*

- `x`: Same data type as first template parameter. In above code snippet it is `int`.

*Returns*

`double`: Prediction of `x`.

*Descrption*

Called after the model is fitted, it returns the prediction of single input.



### 5. predictMany()

*Syntax*

```c++
LinearFit<int, float> lf;
lf.fit(X, y, size);

const size_t size = 5;
int xTest[size] = {1, 2, 3, 4, 5};

double yPred[size];
lf.predictMany(xTest, yPred, size);
```

*Parameters*

- `xTest`: Same type as first template parameter. In above code snippet it is `int[]`.

- `yPred`: Type of `double[]`.

*Returns*

`void`

*Descrption*

Called after the model is fitted, it calculates all the predictions and stores in `yPred`.



### 6. MSE()

*Syntax*

```c++
LinearFit<int, float> lf;
lf.fit(X, y, size);

const size_t size = 5;
int xTest[size] = {1, 2, 3, 4, 5};
float yTest[size] = {0.5, 0.8, 0.89, 0.6, 1.2};

double yPred[size];
lf.predictMany(xTest, yPred, size);

double mse = lf.MSE(yTest, yPred, size);
```

*Parameters*

- `yTest`: Same type as first template parameter. In above code snippet it is `float[]`.

- `yPred`: Type of `double[]`.

- `size`: Type of `size_t`.

*Returns*

`double`: Calculated Mean Squared Error value.

*Descrption*

It returns the mean squared error of the fitted model.



### 7. R2Score()

*Syntax*

```c++
LinearFit<int, float> lf;
lf.fit(X, y, size);

const size_t size = 5;
int xTest[size] = {1, 2, 3, 4, 5};
float yTest[size] = {0.5, 0.8, 0.89, 0.6, 1.2};

double yPred[size];
lf.predictMany(xTest, yPred, size);

double r2score = lf.R2Score(yTest, yPred, size);
```

*Parameters*

- `yTest`: Same type as first template parameter. In above code snippet it is `float`.

- `yPred`: Type of `double[]`.

- `size`: Type of `size_t`.

*Returns*

`double`: Calculated coefficient of determination value.

*Descrption*

It returns the coefficient of determination or r squared score of the fitted model.
