#include "kalman.h"

void	parse_data(const char *data, measurement_t *measurements)
{
	measurements->timestamp = atof(data);
	measurements->sensor_id = atoi(data + 8);
	measurements->bearing = atof(data + 10);
}