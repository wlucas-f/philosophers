#include "philo.h"

void	*routine_1(void *arg)
{
	t_philo	*philo;
	t_table	*data;

	philo = arg;
	data = philo->data;
	while (!simulation_status(data))
	{
		eat(data, philo);
		safe_print(data, "is sleeping", philo->id);
		usleep(data->time_to_sleep * 1000);
		safe_print(data, "is thinking", philo->id);
		usleep(500);
	}
	return (NULL);
}
