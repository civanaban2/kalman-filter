#ifndef SIMULATION_H
#define SIMULATION_H

#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

typedef struct	sensor
{
	int		id;
	int		x;
	int		y;
}	sensor_t;

typedef struct	ship
{
	double		x;
	double		y;
	double		vx;
	double		vy;
	double		w;
}	ship_t;

typedef struct	measurement
{
	double	timestamp;
	double	bearing;
	int		sensor_id;
}	measurement_t;

typedef struct	simulation
{
	sensor_t		sensors[3];
	measurement_t	measurements[100];
	ship_t			ship;
	int				num_measurements;
	double			timestamp;
	double			radar_bearing;
}	simulation_t;

void	initialization(simulation_t *sim);
void	record_measurements(simulation_t *sim);
void	sensor_control(simulation_t *sim);
void	update_simulation(simulation_t *sim, double step);
void	write_measurements(simulation_t *sim);
double	randn(int id);

#endif
