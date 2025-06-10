#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>
#include <math.h>

// Matrix operation functions
double	**mat_multiply(double **a, double **b, int rows_a, int cols_a, int cols_b);
double	**mat_add(double **a, double **b, int rows, int cols);
double	**mat_subtract(double **a, double **b, int rows, int cols);
double	**mat_transpose(double **a, int rows, int cols);
double	mat_determinant(double **a, int size);

#endif