#include "matrix.h"

double	mat3x3_determinant(double a[3][3])
{
	return a[0][0] * (a[1][1] * a[2][2] - a[1][2] * a[2][1]) -
	       a[0][1] * (a[1][0] * a[2][2] - a[1][2] * a[2][0]) +
	       a[0][2] * (a[1][0] * a[2][1] - a[1][1] * a[2][0]);
}

double	**mat4x4_multiply(double a[4][4], double b[4][4])
{
	double	**result = (double **)malloc(4 * sizeof(double *));
	for (int i = 0; i < 4; i++)
		result[i] = (double *)malloc(4 * sizeof(double));

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			result[i][j] = a[i][0] * b[0][j] + a[i][1] * b[1][j] +
			                a[i][2] * b[2][j] + a[i][3] * b[3][j];
	return result;
}

double	**mat4x4_add(double a[4][4], double b[4][4])
{
	double	**result = (double **)malloc(4 * sizeof(double *));
	for (int i = 0; i < 4; i++)
		result[i] = (double *)malloc(4 * sizeof(double));

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			result[i][j] = a[i][j] + b[i][j];
	return result;
}

double	**mat4x4_subtract(double a[4][4], double b[4][4])
{
	double	**result = (double **)malloc(4 * sizeof(double *));
	for (int i = 0; i < 4; i++)
		result[i] = (double *)malloc(4 * sizeof(double));

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			result[i][j] = a[i][j] - b[i][j];
	return result;
}

double	**mat4x4_transpose(double a[4][4])
{
	double	**result = (double **)malloc(4 * sizeof(double *));
	for (int i = 0; i < 4; i++)
		result[i] = (double *)malloc(4 * sizeof(double));

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			result[j][i] = a[i][j];
	return result;
}

double	mat4x4_determinant(double a[4][4])
{
	double	det = 0.0;
	for (int i = 0; i < 4; i++)
	{
		double	submatrix[3][3];
		for (int j = 1; j < 4; j++)
			for (int k = 0; k < 4; k++)
				if (k < i)
					submatrix[j - 1][k] = a[j][k];
				else if (k > i)
					submatrix[j - 1][k - 1] = a[j][k];
		det += (i % 2 == 0 ? 1 : -1) * a[0][i] * mat3x3_determinant(submatrix);
	}
	return det;
}

double	**mat4x4_inverse(double a[4][4])
{
	double	det = mat4x4_determinant(a);
	if (det == 0)
		return NULL; // Singular matrix, no inverse

	double	**result = (double **)malloc(4 * sizeof(double *));
	for (int i = 0; i < 4; i++)
		result[i] = (double *)malloc(4 * sizeof(double));

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			double	submatrix[3][3];
			for (int k = 0; k < 4; k++)
				for (int l = 0; l < 4; l++)
					if (k < i && l < j)
						submatrix[k][l] = a[k][l];
					else if (k < i && l > j)
						submatrix[k][l - 1] = a[k][l];
					else if (k > i && l < j)
						submatrix[k - 1][l] = a[k][l];
					else if (k > i && l > j)
						submatrix[k - 1][l - 1] = a[k][l];
			result[j][i] = ((i + j) % 2 == 0 ? 1 : -1) * mat3x3_determinant(submatrix) / det;
		}
	return result;
}

double **get_F_matrix(double dt)
{
	double	**F = (double **)malloc(4 * sizeof(double *));
	for (int i = 0; i < 4; i++)
		F[i] = (double *)malloc(4 * sizeof(double));

	F[0][0] = 1.0; F[0][1] = 0.0; F[0][2] = dt; F[0][3] = 0.0;
	F[1][0] = 0.0; F[1][1] = 1.0; F[1][2] = 0.0; F[1][3] = dt;
	F[2][0] = 0.0; F[2][1] = 0.0; F[2][2] = 1.0; F[2][3] = 0.0;
	F[3][0] = 0.0; F[3][1] = 0.0; F[3][2] = 0.0; F[3][3] = 1.0;

	return F;
}

