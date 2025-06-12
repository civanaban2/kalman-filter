#include <math.h>
#include <stdio.h>
#include "kalman.h"

void gauss_newton(kalman_t *kalman, measurement_t *measurements)
{
	const int max_iter = 100;
	const double tol = 1e-6;

	// Compute initial guess as average of sensor positions
	double sx_sum = 0.0, sy_sum = 0.0;
	for (int i = 0; i < 3; ++i) {
		sx_sum += kalman->sensor_pos[i][0];
		sy_sum += kalman->sensor_pos[i][1];
	}
	kalman->x[0] = sx_sum / 3.0;
	kalman->x[1] = sy_sum / 3.0;

	for (int iter = 0; iter < max_iter; ++iter)
	{
		double fx[3], H[3][2];
		for (int i = 0; i < 3; ++i)
		{
			double dx = x - kalman->sensor_pos[i][0];
			double dy = y - kalman->sensor_pos[i][1];
			double r2 = dx * dx + dy * dy;
			if (r2 < 1e-8) 
				r2 = 1e-8; // avoid division by zero

			double predicted_bearing = atan2(dy, dx);
			double angle_diff = predicted_bearing - measurements[i].bearing * M_PI / 180.0; // Convert bearing to radians

			// Normalize angle_diff to [-pi, pi]
			angle_diff = fmod(angle_diff + M_PI, 2 * M_PI);
			if (angle_diff < 0) 
				angle_diff += 2 * M_PI;
			angle_diff -= M_PI;

			fx[i] = angle_diff;

			H[i][0] = -dy / r2;
			H[i][1] = dx / r2;
		}

		// Compute H^T * H and H^T * fx
		double H_transpose[2][3] = mat_transpose(H, 3, 2);
		H = mat_multiply(H_transpose, H, 2, 3, 2);
		double det = mat_determinant(H, 2);
		if (fabs(det) < 1e-12)
			break;
		double inv[2][2];
		inv[0][0] = H[1][1] / det;
		inv[0][1] = -H[0][1] / det;
		inv[1][0] = -H[1][0] / det;
		inv[1][1] = H[0][0] / det;

		H = mat_multiply(inv, H_transpose, 2, 2, 3);
		H = mat_multiply(H, fx, 2, 3, 1);
		kalman->x[0] -= H[0];
		kalman->x[1] -= H[1];
		kalman->x[2] = 0;
		kalman->x[3] = 0;
		if (sqrt(H[0][0] * H[0][0] + H[1][0] * H[1][0]) < tol)
			break;
	}
}