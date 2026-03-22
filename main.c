#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct s_table
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
} t_table;

typedef struct s_philo
{
	int id;
	pthread_t thread;
	int left_fork;
	int right_fork;
	long	last_meal_time;
	int		meals_eaten;
	pthread_mutex_t	meal_mutex;
	t_table *data;
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


int parse(t_table *data, int ac, char **av)
{
	if(validate_input(ac, av))
		return(1);
	data->nb_philo = 0;
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

int init_philo(t_table *data, t_philo *philos)
{
	int i;
	pthread_t *th;
	
	th = malloc(sizeof(pthread_t) * data->nb_philo);
	if(!th)
		return(1);
	i = 0;
	while(i < data->nb_philo)	
	{
		philos[i].id = i+1;
		philos[i].left_fork = i;
		philos[i].right_fork = (i + 1) % data->nb_philo;
		philos[i].thread = th[i];
		philos[i].data = data;
		philos[i].meals_eaten = 0;
		philos[i].last_meal_time = get_time();
		i++;
	}
	free(th);
	return(0);
}

long timestamp_in_ms(t_table *data)
{
	return((get_time() - data->start_time));
}

int simulation_status(t_table *data)
{
	int ret;

	ret = 0;
	pthread_mutex_lock(&data->print_mutex);
	ret = data->simulation_end;
	pthread_mutex_unlock(&data->print_mutex);
	return (ret);
}

void safe_print(t_table *data, char *msg, int id)
{
	pthread_mutex_lock(&data->print_mutex);
	if(!data->simulation_end)
		printf("%ld %d %s\n", timestamp_in_ms(data), id, msg);
	pthread_mutex_unlock(&data->print_mutex);
}

void eat(t_table *data, t_philo *philo)
{
	int last;
	int first;

	last = philo->left_fork;
	first = philo->right_fork;
	if(philo->id % 2)
	{
		first = philo->left_fork;
		last = philo->right_fork;
	}
	pthread_mutex_lock(&data->forks[first]);
	safe_print(data, "has taken a fork", philo->id);
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

void *routine_1(void *arg)
{
	t_philo *philo = arg;
	t_table *data = philo->data;
	int id = philo->id;
	
	while(!simulation_status(data))
	{
		eat(data, philo);
		safe_print(data, "is sleeping", id);
		usleep(data->time_to_sleep *1000);
		safe_print(data, "is thinking", id);
		usleep(500);
	}
	return NULL;
}

int is_starving(t_table *data, t_philo philo)
{
	int ret;

	ret = 0;
	pthread_mutex_lock(&philo.meal_mutex);
	pthread_mutex_lock(&data->print_mutex);
	if((get_time() - philo.last_meal_time > data->time_to_die))
		ret = 1;
	pthread_mutex_unlock(&philo.meal_mutex);
	pthread_mutex_unlock(&data->print_mutex);
	return (ret);
}
void death(t_table *data, t_philo philo)
{
	safe_print(data, "has died", philo.id);
	pthread_mutex_lock(&data->print_mutex);
	data->simulation_end = 1;
	pthread_mutex_unlock(&data->print_mutex);
}

void *routine_2(void *arg)
{
	t_table *data = arg;
	t_philo *philos = data->philos;
	int i;

	while(!simulation_status(data))
	{
		i = 0;
		while(i < data->nb_philo)
		{
			if(is_starving(data, philos[i]))
			{
				death(data, philos[i]);
				return NULL;
			}
			i++;
		}
		usleep(1000);
	}
	return NULL;
}

void start(t_table *data, t_philo *philos)
{
	pthread_t monitor;
	int i;

	i = 0;
	while(i < data->nb_philo)
	{
		if(pthread_create(&philos[i].thread, NULL, &routine_1, &philos[i]))
			return ;
		i++;
	}
	pthread_mutex_lock(&data->print_mutex);
	data->start_time = get_time();
	pthread_mutex_unlock(&data->print_mutex);
	if(pthread_create(&monitor, NULL, &routine_2, &*data))
		return ;
	i = 0;
	while(i < data->nb_philo)
	{
		if(pthread_join(philos[i].thread, NULL))
			return ;
		i++;
	}
	if(pthread_join(monitor, NULL))
		return ;
}

void init_mutex(t_table *data, t_philo *philo)
{
	int i = 0;
	while(i < data->nb_philo)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		pthread_mutex_init(&philo[i].meal_mutex, NULL);
		i++;
	}
	pthread_mutex_init(&data->print_mutex, NULL);
	pthread_mutex_init(&data->death_mutex, NULL);
}

void destroy_mutex(t_table *data, t_philo *philo)
{
	int i = 0;
	while(i < data->nb_philo)
	{
		pthread_mutex_destroy(&data->forks[i]);
		pthread_mutex_destroy(&philo[i].meal_mutex);
		i++;
	}
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->death_mutex);
}

int main(int ac, char **av)
{
	t_table *data;
	t_philo *philos;

	if(ac < 5 || ac > 6)
		return 1;
	data = malloc(sizeof(t_table));
	if(!data)
		return (1);
	if	(parse(data, ac, av))
		return(1);
	philos = data->philos;
	init_mutex(data, philos);
	init_philo(data, philos);
	start(data, philos);
	destroy_mutex(data, philos);
	free(data->forks);
	free(philos);
	free(data);
}
