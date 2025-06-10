#include "simulation.h"

void	write_measurements(simulation_t *sim)
{
	int		fd;
	int		i = 0;
	char	buffer[22];

	fd = open("data.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	printf("Simulation starting in 3 seconds...\n");
	sleep(3);
	printf("Simulation started with timestamp 0.0.\n");
	while (i < sim->num_measurements)
	{
		if (i == 0)
			usleep(sim->measurements[i].timestamp * 1000000);
		else
			usleep((sim->measurements[i].timestamp - sim->measurements[i - 1].timestamp) * 1000000);
		if (rand() % 10 == 0)
		{
			i++;
			continue;
		}
		snprintf(buffer, sizeof(buffer), "%-7.2f %d %10.4f\n",
			sim->measurements[i].timestamp,
			sim->measurements[i].sensor_id,
			sim->measurements[i].bearing + randn(sim->measurements[i].sensor_id));
		write(fd, buffer, strlen(buffer));
		write(2, buffer, strlen(buffer));
		i++;
	}
	close(fd);
}

double randn(int id) {
    
	double stddev = 0.0;

	if (id == 1)
		stddev = 0.5;
	else if (id == 2)
		stddev = 1.0;
	else if (id == 3)
		stddev = 1.5;

	double u1 = (rand() + 1.0) / (RAND_MAX + 1.0);
    double u2 = (rand() + 1.0) / (RAND_MAX + 1.0);
    double z = sqrt(-2.0 * log(u1)) * cos(2 * M_PI * u2);
    return z * stddev;
}
