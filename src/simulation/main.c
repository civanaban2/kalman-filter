#include "simulation.h"

int main(void)
{
	srand(time(NULL));
	simulation_t	sim;

	initialization(&sim);
	record_measurements(&sim);
	write_measurements(&sim);
	free(sim.sensors);
	free(sim.measurements);
	printf("Simulation completed successfully.\n");
	return 0;
}
