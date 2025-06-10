#include <math.h>
#include <stdio.h>
#include "kalman.h"

void gauss_newton(kalman_t *kalman, measurement_t *measurements)
{
	const int max_iter = 10;
	const double tol = 1e-6;

	// Compute initial guess as average of sensor positions
	double sx_sum = 0.0, sy_sum = 0.0;
	for (int i = 0; i < 3; ++i) {
		sx_sum += kalman->sensor_pos[i][0];
		sy_sum += kalman->sensor_pos[i][1];
	}
	double x = sx_sum / 3.0;
	double y = sy_sum / 3.0;

	for (int iter = 0; iter < max_iter; ++iter)
	{
		double fx[3], H[3][2];
		for (int i = 0; i < 3; ++i)
		{
			double dx = x - kalman->sensor_pos[i][0];
			double dy = y - kalman->sensor_pos[i][1];
			double r2 = dx * dx + dy * dy;
			if (r2 < 1e-8) r2 = 1e-8; // avoid division by zero

			double predicted_bearing = atan2(dy, dx);
			double angle_diff = predicted_bearing - measurements[i].bearing;

			// Ensure bearing differences are in radians
			while (angle_diff > M_PI) angle_diff -= 2 * M_PI;
			while (angle_diff < -M_PI) angle_diff += 2 * M_PI;

			fx[i] = angle_diff;

			H[i][0] = -dy / r2;
			H[i][1] = dx / r2;
		}

		// Compute H^T * H and H^T * fx
		double HtH[2][2] = {0}, Htf[2] = {0};
		for (int i = 0; i < 3; ++i)
		{
			HtH[0][0] += H[i][0] * H[i][0];
			HtH[0][1] += H[i][0] * H[i][1];
			HtH[1][0] += H[i][1] * H[i][0];
			HtH[1][1] += H[i][1] * H[i][1];
			Htf[0] += H[i][0] * fx[i];
			Htf[1] += H[i][1] * fx[i];
		}

		double det = HtH[0][0] * HtH[1][1] - HtH[0][1] * HtH[1][0];
		if (fabs(det) < 1e-12)
			break;

		double inv[2][2];
		inv[0][0] = HtH[1][1] / det;
		inv[0][1] = -HtH[0][1] / det;
		inv[1][0] = -HtH[1][0] / det;
		inv[1][1] = HtH[0][0] / det;

		double delta_x = -(inv[0][0] * Htf[0] + inv[0][1] * Htf[1]);
		double delta_y = -(inv[1][0] * Htf[0] + inv[1][1] * Htf[1]);

		x += delta_x;
		y += delta_y;

		if (sqrt(delta_x * delta_x + delta_y * delta_y) < tol)
			break;
	}

	kalman->x[0] = x;
	kalman->x[1] = y;
	kalman->x[2] = 0.0;
	kalman->x[3] = 0.0;
	printf("Gauss-Newton result: x = %.2f, y = %.2f\n", x, y);
}