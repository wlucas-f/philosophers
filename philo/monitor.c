/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wlucas-f <wlucas-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 15:08:15 by wlucas-f          #+#    #+#             */
/*   Updated: 2026/03/26 15:08:17 by wlucas-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	simulation_status(t_table *data)
{
	int	ret;

	ret = 0;
	pthread_mutex_lock(&data->print_mutex);
	ret = data->simulation_end;
	pthread_mutex_unlock(&data->print_mutex);
	return (ret);
}

int	meals_reached(t_table *data, t_philo *philo)
{
	int	ret;
	int	full_philos;
	int	i;

	full_philos = 0;
	ret = 0;
	i = -1;
	pthread_mutex_lock(&philo->meal_mutex);
	while (++i < data->nb_philo)
	{
		if (philo[i].meals_eaten >= data->max_meals)
			full_philos += 1;
	}
	pthread_mutex_unlock(&philo->meal_mutex);
	if (data->nb_philo == full_philos)
		ret = 1;
	return (ret);
}

int	is_starving(t_table *data, t_philo philo)
{
	int	ret;

	ret = 0;
	pthread_mutex_lock(&philo.meal_mutex);
	pthread_mutex_lock(&data->print_mutex);
	if ((get_time() - philo.last_meal_time > data->time_to_die))
		ret = 1;
	pthread_mutex_unlock(&philo.meal_mutex);
	pthread_mutex_unlock(&data->print_mutex);
	return (ret);
}

void	death(t_table *data, t_philo philo)
{
	safe_print(data, "has died", philo.id);
	pthread_mutex_lock(&data->print_mutex);
	data->simulation_end = 1;
	pthread_mutex_unlock(&data->print_mutex);
}

void	*routine_2(void *arg)
{
	t_table	*data;
	int		i;

	data = arg;
	while (!simulation_status(data))
	{
		i = -1;
		while (++i < data->nb_philo)
		{
			if (data->max_meals != -1 && meals_reached(data, data->philos))
			{
				pthread_mutex_lock(&data->print_mutex);
				data->simulation_end = 1;
				pthread_mutex_unlock(&data->print_mutex);
			}
			if (is_starving(data, data->philos[i]))
			{
				death(data, data->philos[i]);
				return (NULL);
			}
		}
		usleep(1000);
	}
	return (NULL);
}
