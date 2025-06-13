#include "kalman.h"

void	kalman_filter(kalman_t *kalman, const char *data, int fd)
{
	static int				measurement_count = 0;
    static measurement_t	last_data;
    static measurement_t	first_three[3];
    measurement_t			new_data;

    if (measurement_count < 3)
    {
        parse_data(data, &first_three[measurement_count]);
        measurement_count++;
        
        if (measurement_count == 3)
        {
            gauss_newton(kalman, first_three);
			printf("Initial position: x = %.2f, y = %.2f\n", kalman->x[0], kalman->x[1]);
			printf("Timestamp | x_estimate | y_estimate | vx_estimate | vy_estimate\n");
			last_data = first_three[2];
        }
    }
    else
    {
        parse_data(data, &new_data);
        ekf(kalman, new_data, last_data);
		printf("%-9.2f   %-10.4f   %-10.4f   %-11.4f   %-11.4f\n", new_data.timestamp, kalman->x[0], kalman->x[1], kalman->x[2], kalman->x[3]);
		dprintf(fd, "%-9.2f   %-10.4f   %-10.4f   %-11.4f   %-11.4f\n", new_data.timestamp, kalman->x[0], kalman->x[1], kalman->x[2], kalman->x[3]);
        last_data = new_data;
    }
}