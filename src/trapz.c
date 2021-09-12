#include "trapz.h"

#include <math.h>

double trapz1( double y[], int N)
{
  double sum = 0;
  for (int i = 0; i < N - 1; ++i)
    sum += (y[i] + y[i + 1]) / 2;
  return sum;
}

double trapz2(double x[], double y[], int N)
{
  double sum = 0;
  for (int i = 0; i < N - 1; ++i)
    sum += (y[i] + y[i + 1]) / 2 * fabs(x[i + 1] - x[i]);
  return sum;
}