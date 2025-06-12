#include "kalman.h"

void	kalman_initialization(kalman_t *kalman)
{
	get_Q_matrix(kalman->Q);
	get_sensors(kalman->sensor_pos);
}

void	get_sensors(double sensors[3][2])
{
	sensors[0][0] = 0.0;	sensors[0][1] = 0.0;
	sensors[1][0] = 500.0;	sensors[1][1] = 0.0;
	sensors[2][0] = 250.0;	sensors[2][1] = 400.0;
}

void	get_Q_matrix(double Q[4][4])
{
	Q[0][0] = 10.0;		Q[0][1] = 0.0;		Q[0][2] = 0.0;		Q[0][3] = 0.0;
	Q[1][0] = 0.0;		Q[1][1] = 10.0;		Q[1][2] = 0.0;		Q[1][3] = 0.0;
	Q[2][0] = 0.0;		Q[2][1] = 0.0;		Q[2][2] = 10.0;		Q[2][3] = 0.0;
	Q[3][0] = 0.0;		Q[3][1] = 0.0;		Q[3][2] = 0.0;		Q[3][3] = 10.0;
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
