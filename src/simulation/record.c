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
			if (sim->num_measurements > 0
				&& sim->measurements[sim->num_measurements - 1].sensor_id == sensor.id)
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
