#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

typedef struct s_data
{
	int nb_philo;
	long time_to_die;
	long time_to_eat;
	long time_to_sleep;
	int	max_meals;
	long start_time;
	int simulation_end;
	pthread_mutex_t *forks;
	pthread_mutex_t print_mutex;
	pthread_mutex_t death_mutex;
	struct s_philo *philos;
} t_data;

typedef struct s_philo
{
	int id;
	pthread_t thread;
	int left_fork;
	int right_fork;
	long	last_meal_time;
	int		meals_eaten;
	pthread_mutex_t	meal_mutex;
	t_data *data;
} t_philo;


int is_numeric(char *s)
{
	while(*s)
	{
		if (*s < '0' || *s >'9')
			return (0);
		s++;
	}
	return(1);
}

int validate_input(int ac, char **av)
{
	int i;

	i = 0;
	while(i < ac - 1)
	{
		if(!is_numeric(av[i + 1]))
		{
			write(2, "Error: invalid input\n", 21);
			return(1);
		}
		i++;
	}
	return(0);
}

	long start_time;
	int simulation_end;
	pthread_mutex_t *forks;
	pthread_mutex_t print_mutex;
	pthread_mutex_t death_mutex;
	struct s_philo *philos;

int parse(t_data *data, int ac, char **av)
{
	if(validate_input(ac, av))
		return(1);
	data->nb_philo = atoi(av[1]);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_philo);
	if(!data->forks)
		return(1);
	data->time_to_die = atoi(av[2]);
	data->time_to_eat = atoi(av[3]);
	data->time_to_sleep = atoi(av[4]);
	data->max_meals = -1;
	if (ac == 6)
		data->max_meals = atoi(av[5]);
	return(0);
}


int main(int ac, char **av)
{
	t_data *data;
	pthread_t th;

	if(ac < 5 || ac > 6)
		return 1;
	data = malloc(sizeof(t_data));
	if(!data)
		return (1);
	if	(parse(data, ac, av))
		return(1);
	if (init())
		return(1);
	
	printf("number_of_philosophers: %d\n", data->nb_philo);
	printf("time_to_die: %ld\n", data->time_to_die);
	printf("time_to_eat: %ld\n", data->time_to_eat);
	printf("time_to_sleep: %ld\n", data->time_to_sleep);
	if (data->max_meals > 0)
		printf("number_of_times_each_philosopher_must_eat: %d\n", data->max_meals);
}
