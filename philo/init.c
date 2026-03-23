#include "philo.h"

int	init_philo(t_table *data, t_philo *philos)
{
	int			i;
	pthread_t	*th;

	th = malloc(sizeof(pthread_t) * data->nb_philo);
	if (!th)
		return (1);
	i = 0;
	while (i < data->nb_philo)
	{
		philos[i].id = i + 1;
		philos[i].left_fork = i;
		philos[i].right_fork = (i + 1) % data->nb_philo;
		philos[i].thread = th[i];
		philos[i].data = data;
		philos[i].meals_eaten = 0;
		philos[i].last_meal_time = get_time();
		i++;
	}
	free(th);
	return (0);
}

void	init_mutex(t_table *data, t_philo *philo)
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		pthread_mutex_init(&philo[i].meal_mutex, NULL);
		i++;
	}
	pthread_mutex_init(&data->print_mutex, NULL);
}
