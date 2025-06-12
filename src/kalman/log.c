#include "kalman.h"

void logger_write(kalman_t *kalman, measurement_t *new_data) {
	static int init;
    int fd = open("log.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0) {
		perror("Failed to open log file");
		return;
	}
	if (!init) {
		dprintf(fd, "%-9s %-9s %-9s %-9s %-9s\n",
			"x_true", "y_true", "x_est", "y_est", "error2");
		init = 1;
	}
	double x_true = new_data->timestamp * 5 + 100;
	double y_true = new_data->timestamp * 3 + 200;
	double error2 = (kalman->x[0] - x_true) * (kalman->x[0] - x_true) +
					(kalman->x[1] - y_true) * (kalman->x[1] - y_true);
	dprintf(fd, "%-9.4f %-9.4f %-9.4f %-9.4f %-9.4f\n", x_true, y_true,
		kalman->x[0], kalman->x[1], error2);
	close(fd);
}