/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wlucas-f <wlucas-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 15:08:23 by wlucas-f          #+#    #+#             */
/*   Updated: 2026/03/26 15:08:26 by wlucas-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(char *s)
{
	int	i;
	int	res;

	i = 0;
	res = 0;
	while (s[i] == ' ' || (s[i] >= 9 && s[i] <= 13))
		i++;
	while (s[i] >= '0' && s[i] <= '9')
	{
		res = res * 10 + s[i] - '0';
		i++;
	}
	return (res);
}

int	parse(t_table *data, int ac, char **av)
{
	data->nb_philo = 0;
	data->nb_philo = ft_atoi(av[1]);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_philo);
	if (!data->forks)
		return (1);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	data->max_meals = -1;
	if (ac == 6)
		data->max_meals = ft_atoi(av[5]);
	data->start_time = get_time();
	data->simulation_end = 0;
	data->philos = malloc(sizeof(t_philo) * data->nb_philo);
	if (!data->philos)
		return (1);
	return (0);
}

void	start(t_table *data, t_philo *philos)
{
	pthread_t	monitor;
	int			i;

	i = 0;
	pthread_mutex_lock(&data->print_mutex);
	data->start_time = get_time();
	pthread_mutex_unlock(&data->print_mutex);
	while (i < data->nb_philo)
	{
		if (pthread_create(&philos[i].thread, NULL, &routine_1, &philos[i]))
			return ;
		i++;
	}
	if (pthread_create(&monitor, NULL, &routine_2, &*data))
		return ;
	i = 0;
	while (i < data->nb_philo)
	{
		if (pthread_join(philos[i].thread, NULL))
			return ;
		i++;
	}
	if (pthread_join(monitor, NULL))
		return ;
}

int	main(int ac, char **av)
{
	t_table	*data;
	t_philo	*philos;

	if (ac < 5 || ac > 6 || validate_input(ac, av))
		return (1);
	data = malloc(sizeof(t_table));
	if (!data)
		return (1);
	if (parse(data, ac, av))
		return (1);
	philos = data->philos;
	init_mutex(data, philos);
	init_philo(data, philos);
	start(data, philos);
	destroy_mutex(data, philos);
	free(data->forks);
	free(philos);
	free(data);
}
