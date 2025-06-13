#include "kalman.h"

void	kalman_initialization(kalman_t *kalman)
{
	get_Q_matrix(kalman->Q);
	get_sensors(kalman->sensor_pos);
	get_P_matrix(kalman->P);
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

void get_F_matrix(double dt, kalman_t *kalman)
{
	kalman->F[0][0] = 1.0;	kalman->F[0][1] = 0.0;	kalman->F[0][2] = dt;	kalman->F[0][3] = 0.0;
	kalman->F[1][0] = 0.0;	kalman->F[1][1] = 1.0;	kalman->F[1][2] = 0.0;	kalman->F[1][3] = dt;
	kalman->F[2][0] = 0.0;	kalman->F[2][1] = 0.0;	kalman->F[2][2] = 1.0;	kalman->F[2][3] = 0.0;
	kalman->F[3][0] = 0.0;	kalman->F[3][1] = 0.0;	kalman->F[3][2] = 0.0;	kalman->F[3][3] = 1.0;
}

void get_P_matrix(double P[4][4])
{
	P[0][0] = 10.0;	P[0][1] = 0.0;		P[0][2] = 0.0;		P[0][3] = 0.0;
	P[1][0] = 0.0;		P[1][1] = 10.0;	P[1][2] = 0.0;		P[1][3] = 0.0;
	P[2][0] = 0.0;		P[2][1] = 0.0;		P[2][2] = 100.0;	P[2][3] = 0.0;
	P[3][0] = 0.0;		P[3][1] = 0.0;		P[3][2] = 0.0;		P[3][3] = 100.0;
}

void get_H_kalman(kalman_t *kalman, measurement_t *measurement)
{
	kalman->H[0][0] = (-kalman->x[1] + kalman->sensor_pos[measurement->sensor_id - 1][1]) /
		(pow(kalman->x[0] - kalman->sensor_pos[measurement->sensor_id - 1][0], 2) +
		pow(kalman->x[1] - kalman->sensor_pos[measurement->sensor_id - 1][1], 2));
	kalman->H[0][1] = (kalman->x[0] - kalman->sensor_pos[measurement->sensor_id - 1][0]) /
		(pow(kalman->x[0] - kalman->sensor_pos[measurement->sensor_id - 1][0], 2) +
		pow(kalman->x[1] - kalman->sensor_pos[measurement->sensor_id - 1][1], 2));
	kalman->H[0][2] = 0.0;
	kalman->H[0][3] = 0.0;
}

void get_R_matrix(kalman_t *kalman, measurement_t *measurement)
{
	kalman->R[0][0] = (measurement->sensor_id * 0.5) * (measurement->sensor_id * 0.5);
}