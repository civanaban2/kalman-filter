#ifndef KALMAN_H
#define KALMAN_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>

typedef struct kalman_filter {
	double	x[4];
	double	P[4][4];
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

typedef struct gauss_newton {
	int		max_iter;
	double	tol;
	double	x;
	double	y;
	double	sx_sum;
	double	sy_sum;
	double	dx;
	double	dy;
	double	r2;
	double	fx[3];
	double	H[3][2];
	double	predicted_bearing;
	double	angle_diff;
	double	inv[2][2];
	double	det;
	double	delta_x;
	double	delta_y;
}	gauss_newton_t;

void	kalman_filter(kalman_t *kalman, const char *data, int fd);
void	kalman_initialization(kalman_t *kalman);
void	parse_data(const char *data, measurement_t *measurement);
void	get_P_matrix(double P[4][4]);
void	get_Q_matrix(double Q[4][4]);
void	get_sensors(double sensors[3][2]);
void	get_F_matrix(double dt, kalman_t *kalman);
void	gauss_newton(kalman_t *kalman, measurement_t *measurements);
void	gauss_init(gauss_newton_t *gn);
void	get_H_matrix(gauss_newton_t *gn, kalman_t *kalman, measurement_t *measurements);
void	HtH_and_Htf(double HtH[2][2], double Htf[2], gauss_newton_t *gn);
void	get_inv(gauss_newton_t *gn, double HtH[2][2]);
void	ekf(kalman_t *kalman, measurement_t new_data, measurement_t last_data);
void	ekf_predict(kalman_t *kalman, measurement_t new_data, measurement_t last_data);
void	ekf_update(kalman_t *kalman, measurement_t new_data);
void	get_H_kalman(kalman_t *kalman, measurement_t *measurement);
void	get_R_matrix(kalman_t *kalman, measurement_t *measurement);

#endif
