#ifndef KALMAN_H
#define KALMAN_H

#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>

typedef struct kalman_filter {
	double	x[4];
	double 	P[4][4];
	double	F[4][4];
	double	Q[4][4];
	double	H[1][4];
	double	R[1][1];
	double	y;
	double	S;
	double	K[4][1];
	double	sensor_pos[3][2];
}	kalman_t;

typedef struct measurement {
	double	timestamp;
	int		sensor_id;
	double	bearing;
}	measurement_t;

// Main kalman functions
void	kalman_filter(kalman_t *kalman, const char *data);
void	kalman_initialization(kalman_t *kalman);

// Parser functions
void	parse_data(const char *data, measurement_t *measurement);

// Initialization functions
void	get_P_matrix(double P[4][4]);
void	get_Q_matrix(double Q[4][4]);
void	get_sensors(double sensors[3][2]);
double **get_F_matrix(double dt);

// Algorithm functions
void	gauss_newton(kalman_t *kalman, measurement_t *measurements);
void	ekf(kalman_t *kalman, measurement_t new_data, measurement_t last_data);

#endif
