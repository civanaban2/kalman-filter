#include "simulation.h"

void	record_measurements(simulation_t *sim)
{
	double	step = 0.0001;
	while (1)
	{
		sensor_control(sim);
		if (sim->num_measurements >= 100)
			break;
		update_simulation(sim, step);
	}
}

void	write_measurements(simulation_t *sim)
{
	int		fd;
	int		i;
	char	buffer[100];

	fd = open("data.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	
	i = 0;
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

void	sensor_control(simulation_t *sim)
{
	double		ship_bearing;
	sensor_t	sensor;
	for (int i = 0; i < 3; i++)
	{
		sensor = sim->sensors[i];
		ship_bearing = atan2(sim->ship.y - sensor.y, sim->ship.x - sensor.x);
		if (fabs(ship_bearing - sim->radar_bearing) < 0.0001)
		{
			if (sim->num_measurements > 0 && sim->measurements[sim->num_measurements - 1].sensor_id == sensor.id)
				continue;
			sim->measurements[sim->num_measurements].timestamp = sim->timestamp;
			sim->measurements[sim->num_measurements].bearing = ship_bearing * 180 / M_PI;
			sim->measurements[sim->num_measurements].sensor_id = sensor.id;
			sim->num_measurements++;
		}
	}
}

void	update_simulation(simulation_t *sim, double step)
{
	sim->timestamp += step;
	sim->ship.x += sim->ship.vx * step;
	sim->ship.y += sim->ship.vy * step;
	sim->radar_bearing += sim->ship.w * step;
	if (sim->radar_bearing > M_PI)
		sim->radar_bearing -= 2 * M_PI;
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
