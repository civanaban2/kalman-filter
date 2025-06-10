#include "matrix.h"

double	**mat_multiply(double **a, double **b, int rows_a, int cols_a, int cols_b)
{
	double	**result = (double **)malloc(rows_a * sizeof(double *));
	for (int i = 0; i < rows_a; i++)
		result[i] = (double *)malloc(cols_b * sizeof(double));
	for (int i = 0; i < rows_a; i++)
		for (int j = 0; j < cols_b; j++)
		{
			result[i][j] = 0.0;
			for (int k = 0; k < cols_a; k++)
				result[i][j] += a[i][k] * b[k][j];
		}
	return result;
}

double	**mat_add(double **a, double **b, int rows, int cols)
{
	double	**result = (double **)malloc(rows * sizeof(double *));
	for (int i = 0; i < rows; i++)
		result[i] = (double *)malloc(cols * sizeof(double));
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			result[i][j] = a[i][j] + b[i][j];
	return result;
}

double	**mat_subtract(double **a, double **b, int rows, int cols)
{
	double	**result = (double **)malloc(rows * sizeof(double *));
	for (int i = 0; i < rows; i++)
		result[i] = (double *)malloc(cols * sizeof(double));
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			result[i][j] = a[i][j] - b[i][j];
	return result;
}

double	**mat_transpose(double **a, int rows, int cols)
{
	double	**result = (double **)malloc(cols * sizeof(double *));
	for (int i = 0; i < cols; i++)
		result[i] = (double *)malloc(rows * sizeof(double));

	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			result[j][i] = a[i][j];
	return result;
}

double	mat_determinant(double **a, int size)
{
	double	det = 0.0;

	if (size == 1)
		return a[0][0];
	if (size == 2)
		return a[0][0] * a[1][1] - a[0][1] * a[1][0];
	for (int i = 0; i < size; i++)
	{
		double	**submatrix = (double **)malloc((size - 1) * sizeof(double *));
		for (int j = 0; j < size - 1; j++)
			submatrix[j] = (double *)malloc((size - 1) * sizeof(double));

		for (int j = 1; j < size; j++)
			for (int k = 0; k < size; k++)
				if (k < i)
					submatrix[j - 1][k] = a[j][k];
				else if (k > i)
					submatrix[j - 1][k - 1] = a[j][k];

		det += (i % 2 == 0 ? 1 : -1) * a[0][i] * mat_determinant(submatrix, size - 1);
		for (int j = 0; j < size - 1; j++)
			free(submatrix[j]);
		free(submatrix);
	}
	return det;
}
