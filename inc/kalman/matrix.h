#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>
#include <math.h>

// Matrix operation functions
double	**mat4x4_multiply(double a[4][4], double b[4][4]);
double	**mat4x4_add(double a[4][4], double b[4][4]);
double	**mat4x4_subtract(double a[4][4], double b[4][4]);
double	**mat4x4_transpose(double a[4][4]);
double	mat4x4_determinant(double a[4][4]);
double	**mat4x4_inverse(double a[4][4]);
double	mat3x3_determinant(double a[3][3]);

#endif