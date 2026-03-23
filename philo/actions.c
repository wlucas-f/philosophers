#include "philo.h"

void	assing_forks(t_philo *philo, int *first, int *last)
{
	*last = philo->left_fork;
	*first = philo->right_fork;
	if (philo->id % 2)
	{
		*first = philo->left_fork;
		*last = philo->right_fork;
	}
}

void	eat(t_table *data, t_philo *philo)
{
	int	last;
	int	first;

	assing_forks(philo, &first, &last);
	pthread_mutex_lock(&data->forks[first]);
	safe_print(data, "has taken a fork", philo->id);
	if (data->nb_philo == 1)
	{
		while (!is_starving(data, *philo))
			usleep(500);
		death(data, *philo);
		pthread_mutex_unlock(&data->forks[first]);
		return ;
	}
	pthread_mutex_lock(&data->forks[last]);
	safe_print(data, "has taken a fork", philo->id);
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal_time = get_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_mutex);
	safe_print(data, "is eating", philo->id);
	usleep(data->time_to_eat * 1000);
	pthread_mutex_unlock(&data->forks[first]);
	pthread_mutex_unlock(&data->forks[last]);
}
