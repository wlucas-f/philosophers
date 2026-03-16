#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

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

long get_time(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
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
	data->start_time = get_time();
	data->simulation_end = 0;
	data->philos = malloc(sizeof(t_philo) * data->nb_philo);
	if(!data->philos)
		return 1;
	return(0);
}

int init_philo(t_data *data, t_philo *philos)
{
	int i;
	pthread_t th[data->nb_philo];

	i = 0;
	while(i < data->nb_philo)	
	{
		philos[i].id = i+1;
		philos[i].thread = th[i];
		philos[i].data = data;
		i++;
	}
	return(0);
}

long timestamp_in_ms(t_data *data)
{
	return((get_time() - data->start_time));
}


void *routine_1(void *data)
{
	t_data *args = (t_data *) data;

	printf("number_of_philosophers: %d\n", args->nb_philo);
	printf("time_to_die: %ld\n", args->time_to_die);
	printf("time_to_eat: %ld\n", args->time_to_eat);
	printf("time_to_sleep: %ld\n", args->time_to_sleep);
	if (args->max_meals > 0)
		printf("number_of_times_each_philosopher_must_eat: %d\n", args->max_meals);
	printf("\n");
	return NULL;
}

void *routine_2(void *arg)
{
	t_philo *philo = arg;
	t_data *data = philo->data;
	int id = philo->id;

	printf("%ld %d has taken the fork\n", timestamp_in_ms(data), id);
	sleep(1);
	printf("%ld %d is eating\n", timestamp_in_ms(data), id);
	sleep(1);
	printf("%ld %d is sleeping\n", timestamp_in_ms(data), id);
	sleep(1);
	printf("%ld %d is thinking\n", timestamp_in_ms(data), id);
	sleep(1);
	printf("%ld %d died\n", timestamp_in_ms(data), id);
	sleep(1);
	return NULL;
}


int main(int ac, char **av)
{
	t_data *data;
	t_philo *philos;
	pthread_t th[5];

	if(ac < 5 || ac > 6)
		return 1;
	data = malloc(sizeof(t_data));
	if(!data)
		return (1);
	if	(parse(data, ac, av))
		return(1);
	philos = data->philos;
	init_philo(data, philos);
	int j = 0;
	for(int i = 0; i < data->nb_philo; i++)
	{
		if(pthread_create(&philos->thread, NULL, &routine_2, philos++) != 0)
			return 1;
		j++;
	}
	for(int i = 0; i < j - 1; i++)
	{
		if(pthread_join((philos++)->thread, NULL) != 0)
			return 1;
	}

	/*
	if(pthread_create(&philo->thread, NULL, &routine_2, philo) != 0)
		return 1;
	if(pthread_join(philo->thread, NULL) != 0)
		return 1;
	*/
	free(data);
//	free(philos);
}
