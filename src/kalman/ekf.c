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
    
    // F matrisinin transpozunu al
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
            Ft[i][j] = kalman->F[j][i];  // Transpoze için i,j yer değiştir
    }

    // x = F * x (state prediction)
    for (int i = 0; i < 4; i++)
    {
        x[i] = 0;  // Sıfırla
        for (int j = 0; j < 4; j++)
            x[i] += kalman->F[i][j] * kalman->x[j];
    }

    // FP = F * P
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            FP[i][j] = 0;  // Sıfırla
            for (int k = 0; k < 4; k++)
                FP[i][j] += kalman->F[i][k] * kalman->P[k][j];
        }
    }

    // P = FP * Ft + Q
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            P[i][j] = kalman->Q[i][j];  // Q ile başlat
            for (int k = 0; k < 4; k++)
                P[i][j] += FP[i][k] * Ft[k][j];
        }
    }

    // Sonuçları kalman yapısına kopyala
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
    double Ht[1][4];
    double K[4][1];
    double HP[1][4] = {0};
    double KH[4][4] = {0};

    h[0] = atan2(kalman->x[1] - kalman->sensor_pos[new_data.sensor_id - 1][1],
                kalman->x[0] - kalman->sensor_pos[new_data.sensor_id - 1][0]);
    y[0] = new_data.bearing - h[0];
    
    // H transpozunu al
    for (int i = 0; i < 4; i++)
    {
        Ht[0][i] = kalman->H[0][i];  // 1x4 matris için
    }
    
    // HP = H * P
    for (int i = 0; i < 1; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            HP[i][j] = 0;
            for (int k = 0; k < 4; k++)
                HP[i][j] += kalman->H[i][k] * kalman->P[k][j];
        }
    }
    
    // S = HP * Ht + R
    for (int i = 0; i < 1; i++)
    {
        for (int j = 0; j < 1; j++)
        {
            S[i][j] = kalman->R[i][j];
            for (int k = 0; k < 4; k++)
                S[i][j] += HP[i][k] * Ht[k][j];
        }
    }
    
    // K = P * Ht / S
    for (int i = 0; i < 4; i++)
    {
        K[i][0] = 0;
        for (int k = 0; k < 1; k++)
            K[i][0] += kalman->P[i][k] * Ht[k][0] / S[0][0];
    }
    
    // x = x + K * y
    for (int i = 0; i < 4; i++)
    {
        kalman->x[i] += K[i][0] * y[0];
    }
    
    // KH = K * H
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            KH[i][j] = 0;
            for (int k = 0; k < 1; k++)
                KH[i][j] += K[i][k] * kalman->H[k][j];
        }
    }
    
    // P = (I - KH) * P
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
    
    // P'yi güncelle
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            kalman->P[i][j] = temp_P[i][j];
            
    logger_write(kalman);
}

void logger_write(kalman_t *kalman) {
	static int init;
    int fd = open("log.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0) {
		perror("Failed to open log file");
		return;
	}
	if (!init) {
		dprintf(fd, "%-9s %-9s\n", "x_est", "y_est");
		init = 1;
	}
	dprintf(fd, "%-9.4f %-9.4f\n", kalman->x[0], kalman->x[1]);
	close(fd);
}