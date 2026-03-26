/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wlucas-f <wlucas-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 15:08:54 by wlucas-f          #+#    #+#             */
/*   Updated: 2026/03/26 15:08:58 by wlucas-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_numeric(char *s)
{
	while (*s)
	{
		if (*s < '0' || *s > '9')
			return (0);
		s++;
	}
	return (1);
}

int	validate_input(int ac, char **av)
{
	int	i;

	i = -1;
	while (++i < ac - 1)
	{
		if (!is_numeric(av[i + 1]))
		{
			write(2, "Error: invalid input\n", 21);
			return (1);
		}
	}
	return (0);
}

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

long	timestamp_in_ms(t_table *data)
{
	return ((get_time() - data->start_time));
}

void	safe_print(t_table *data, char *msg, int id)
{
	pthread_mutex_lock(&data->print_mutex);
	if (!data->simulation_end)
		printf("%ld %d %s\n", timestamp_in_ms(data), id, msg);
	pthread_mutex_unlock(&data->print_mutex);
}
