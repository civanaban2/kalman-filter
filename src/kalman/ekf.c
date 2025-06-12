#include "kalman.h"

void	ekf(kalman_t *kalman, measurement_t new_data, measurement_t last_data)
{
	ekf_predict(kalman, new_data, last_data);
	ekf_update(kalman, new_data, last_data);
	logger_write(kalman, &new_data);
}

void ekf_predict(kalman_t *kalman, measurement_t new_data, measurement_t last_data)
{
	double dt = new_data.timestamp - last_data.timestamp;
	double x[4] ;
	double P[4][4];
	double Ft[4][4];
	double FP[4][4];

	get_F_matrix(dt, kalman);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			Ft[i][j] = kalman->F[i][j];
	}

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			x[i] = kalman->F[i][j] * kalman->x[j];
	}

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			for (int k = 0; k < 4; k++)
				FP[i][j] += kalman->F[i][k] * kalman->P[k][j];
	}

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			for (int k = 0; k < 4; k++)
				P[i][j] += FP[i][k] * Ft[k][j] + kalman->Q[i][j];
	}

	for (int i = 0; i < 4; i++)
		kalman->x[i] = x[i];
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			kalman->P[i][j] = P[i][j];
}

void ekf_update(kalman_t *kalman, measurement_t new_data, measurement_t last_data)
{
	
}