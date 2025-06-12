#include "kalman.h"

void	ekf(kalman_t *kalman, measurement_t new_data, measurement_t last_data)
{
	(void)last_data;
	(void)new_data;
	(void)kalman;
	// ekf_predict(kalman, new_data, last_data);
	// ekf_update(kalman, new_data, last_data);
	// logger_write(kalman, new_data);
}

void ekf_predict(kalman_t *kalman, measurement_t new_data, measurement_t last_data)
{
	(void)last_data;
	(void)new_data;
	(void)kalman;
	// double dt = new_data.timestamp - last_data.timestamp;
	// kalman->F = get_F_matrix(dt);
	// double F_transpose[4][4] = mat_transpose(kalman->F, 4, 4);
	// double x[4];
	// double P[4][4];

	// x = mat_multiply(kalman->F, kalman->x, 4, 4, 1);
	// P = mat_multiply(kalman->F, kalman->P, 4, 4, 4);
	// P = mat_multiply(P, F_transpose, 4, 4, 4);
	// P = mat_add(P, kalman->Q, 4, 4);
	// kalman->x = x;
	// kalman->P = P;
}

void ekf_update(kalman_t *kalman, measurement_t new_data, measurement_t last_data)
{
	(void)last_data;
	(void)new_data;
	(void)kalman;
	// double trust[1][4];
	// double H_transpose[4][1] = mat_transpose(kalman->H, 1, 4);
	// double I[4][4] = mat_identity(4);
	// kalman->R = new_data.sensor_id * 0.5
	// kalman->y = new_data.bearing - atan2(kalman->x[1] - kalman->sensor_pos[new_data.sensor_id][1],
	// 									  kalman->x[0] - kalman->sensor_pos[new_data.sensor_id][0]);
	// trust = mat_multiplication(kalman->H, kalman->P, 1, 4, 4);
	// kalman->S = mat_multiply(trust, H_transpose, 1, 4, 1) + kalman->R[0][0];
	// kalman->K = mat_multiply(kalman->P, H_transpose, 4, 4, 1) / kalman->S;
	// kalman->x = mat_add(kalman->x, mat_multiply(kalman->K, kalman->y, 4, 1, 1), 4, 1);
	// kalman->P = mat_multiply(mat_subtract(I, mat_multiply(kalman->K, kalman->H, 4, 1, 4)), kalman->P, 4, 4, 4);
}