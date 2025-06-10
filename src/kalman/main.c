#include "kalman.h"

int main()
{
	kalman_t	kalman;
	int			fd;
	off_t		last_pos = 0;
	off_t		current_pos = 0;
	char		buffer[22];
	
	fd = open("data.txt", O_RDONLY);
	last_pos = lseek(fd, 0, SEEK_END);
	kalman_initialization(&kalman);
	while(1)
	{
		current_pos = lseek(fd, 0, SEEK_END);

		if (current_pos > last_pos)
		{
			lseek(fd, last_pos, SEEK_SET);
			read(fd, buffer, 22);
			buffer[21] = '\0';
			kalman_filter(&kalman, buffer);
			last_pos = current_pos;
		}
		usleep(100000);
	}
	return 0;
}
