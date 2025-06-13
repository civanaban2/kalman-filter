#include "kalman.h"

void	ekf(kalman_t *kalman, measurement_t new_data, measurement_t last_data)
{
	ekf_predict(kalman, new_data, last_data);
	ekf_update(kalman, new_data);
}

void ekf_predict(kalman_t *kalman, measurement_t new_data, measurement_t last_data)
{
    double dt = new_data.timestamp - last_data.timestamp;
    double x[4] = {0};
    double P[4][4] = {0};
    double Ft[4][4] = {0};
    double FP[4][4] = {0};
    
    get_F_matrix(dt, kalman);
    
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
            Ft[i][j] = kalman->F[j][i];
    }

    for (int i = 0; i < 4; i++)
    {
        x[i] = 0;
        for (int j = 0; j < 4; j++)
            x[i] += kalman->F[i][j] * kalman->x[j];
    }

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            FP[i][j] = 0;
            for (int k = 0; k < 4; k++)
                FP[i][j] += kalman->F[i][k] * kalman->P[k][j];
        }
    }

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            P[i][j] = 0;
            for (int k = 0; k < 4; k++)
                P[i][j] += FP[i][k] * Ft[k][j];
			P[i][j] += kalman->Q[i][j];
        }
    }

    for (int i = 0; i < 4; i++)
        kalman->x[i] = x[i];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            kalman->P[i][j] = P[i][j];
}

void ekf_update(kalman_t *kalman, measurement_t new_data)
{
    double y[1];
    double h[1];
    double S[1][1] = {0};
    double Ht[4][1];
    double K[4][1];
    double HP[1][4] = {0};
    double KH[4][4] = {0};

    h[0] = atan2(kalman->x[1] - kalman->sensor_pos[new_data.sensor_id - 1][1],
                kalman->x[0] - kalman->sensor_pos[new_data.sensor_id - 1][0]);
    y[0] = new_data.bearing * M_PI / 180.0 - h[0];

	get_H_kalman(kalman, &new_data);
    for (int i = 0; i < 4; i++)
    {
        Ht[i][0] = kalman->H[0][i];
    }
    
    for (int i = 0; i < 1; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            HP[i][j] = 0;
            for (int k = 0; k < 4; k++)
                HP[i][j] += kalman->H[i][k] * kalman->P[k][j];
        }
    }
	get_R_matrix(kalman, &new_data);
    S[0][0] = kalman->R[0][0];
    for (int k = 0; k < 4; k++){
        S[0][0] += HP[0][k] * Ht[k][0];
    }

    for (int i = 0; i < 4; i++)
    {
        K[i][0] = 0;
        for (int k = 0; k < 4; k++)
            K[i][0] += kalman->P[i][k] * Ht[k][0];
		K[i][0] /= S[0][0];
    }
    
    for (int i = 0; i < 4; i++)
    {
        kalman->x[i] += K[i][0] * y[0];
    }
    
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            KH[i][j] = 0;
            for (int k = 0; k < 1; k++)
                KH[i][j] += K[i][k] * kalman->H[k][j];
        }
    }
    
    double temp_P[4][4] = {0};
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            double I_KH = (i == j) ? 1.0 - KH[i][j] : -KH[i][j];
            for (int k = 0; k < 4; k++)
                temp_P[i][j] += I_KH * kalman->P[k][j];
        }
    }
    
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            kalman->P[i][j] = temp_P[i][j];
}
