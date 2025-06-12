#include "kalman.h"

void gauss_newton(kalman_t *kalman, measurement_t *measurements)
{
	gauss_newton_t gn;
	double HtH[2][2] = {0};
	double Htf[2] = {0};

	gauss_init(&gn);
	for (int i = 0; i < 3; ++i) {
		gn.sx_sum += kalman->sensor_pos[i][0];
		gn.sy_sum += kalman->sensor_pos[i][1];
	}
	gn.x = gn.sx_sum / 3.0;
	gn.y = gn.sy_sum / 3.0;

	for (int iter = 0; iter < gn.max_iter; ++iter)
	{
		get_H_matrix(&gn, kalman, measurements);
		HtH_and_Htf(HtH, Htf, &gn);
		gn.det = HtH[0][0] * HtH[1][1] - HtH[0][1] * HtH[1][0];
		if (fabs(gn.det) < 1e-12)
			break;

		get_inv(&gn, HtH);

		gn.delta_x = -(gn.inv[0][0] * Htf[0] + gn.inv[0][1] * Htf[1]);
		gn.delta_y = -(gn.inv[1][0] * Htf[0] + gn.inv[1][1] * Htf[1]);
		gn.x += gn.delta_x;
		gn.y += gn.delta_y;
		if (sqrt(gn.delta_x * gn.delta_x + gn.delta_y * gn.delta_y) < gn.tol)
			break;
	}

	kalman->x[0] = gn.x;
	kalman->x[1] = gn.y;
	kalman->x[2] = 0.0;
	kalman->x[3] = 0.0;
}

void	gauss_init(gauss_newton_t *gn)
{
	gn->max_iter = 100;
	gn->tol = 1e-6;
	gn->x = 0.0;
	gn->y = 0.0;
	gn->sx_sum = 0.0;
	gn->sy_sum = 0.0;
	gn->dx = 0.0;
	gn->dy = 0.0;
	gn->r2 = 0.0;
}

void	get_H_matrix(gauss_newton_t *gn, kalman_t *kalman, measurement_t *measurements)
{
	for (int i = 0; i < 3; ++i)
	{
		gn->dx = gn->x - kalman->sensor_pos[measurements[i].sensor_id - 1][0];
		gn->dy = gn->y - kalman->sensor_pos[measurements[i].sensor_id - 1][1];
		gn->r2 = gn->dx * gn->dx + gn->dy * gn->dy;
		gn->predicted_bearing = atan2(gn->dy, gn->dx);
		gn->angle_diff = gn->predicted_bearing - measurements[i].bearing * M_PI / 180.0;
		gn->angle_diff = fmod(gn->angle_diff + M_PI, 2 * M_PI);
		if (gn->angle_diff < 0)
			gn->angle_diff += 2 * M_PI;
		gn->angle_diff -= M_PI;
		gn->fx[i] = gn->angle_diff;
		gn->H[i][0] = -gn->dy / gn->r2;
		gn->H[i][1] = gn->dx / gn->r2;
	}
}

void	HtH_and_Htf(double HtH[2][2], double Htf[2], gauss_newton_t *gn)
{
	for (int i = 0; i < 3; ++i)
	{
		HtH[0][0] += gn->H[i][0] * gn->H[i][0];
		HtH[0][1] += gn->H[i][0] * gn->H[i][1];
		HtH[1][0] += gn->H[i][1] * gn->H[i][0];
		HtH[1][1] += gn->H[i][1] * gn->H[i][1];
		Htf[0] += gn->H[i][0] * gn->fx[i];
		Htf[1] += gn->H[i][1] * gn->fx[i];
	}
}

void	get_inv(gauss_newton_t *gn, double HtH[2][2])
{
	gn->inv[0][0] = HtH[1][1] / gn->det;
	gn->inv[0][1] = -HtH[0][1] / gn->det;
	gn->inv[1][0] = -HtH[1][0] / gn->det;
	gn->inv[1][1] = HtH[0][0] / gn->det;
}