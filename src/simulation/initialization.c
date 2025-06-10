#include "simulation.h"

void	initialization(simulation_t *sim)
{
	sim->sensors[0] = (sensor_t){.id = 1, .x = 0, .y = 0};
	sim->sensors[1] = (sensor_t){.id = 2, .x = 500, .y = 0};
	sim->sensors[2] = (sensor_t){.id = 3, .x = 250, .y = 400};
	sim->ship = (ship_t){.x = 100, .y = 200, .vx = 5, .vy = 3, .w = M_PI / 3};
	sim->timestamp = 0.0;
	sim->radar_bearing = ((double)rand() / RAND_MAX) * 2 * M_PI;
	sim->num_measurements = 0;
}
