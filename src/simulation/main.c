#include "simulation.h"

int	main(void)
{
	simulation_t	sim;

	srand(time(NULL));
	initialization(&sim);
	record_measurements(&sim);
	write_measurements(&sim);
	free(sim.sensors);
	free(sim.measurements);
	printf("Simulation completed successfully.\n");
	return (0);
}
