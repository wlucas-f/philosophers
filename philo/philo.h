#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_table
{
	int				nb_philo;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				max_meals;
	long			start_time;
	int				simulation_end;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	struct s_philo	*philos;
}					t_table;

typedef struct s_philo
{
	int				id;
	pthread_t		thread;
	int				left_fork;
	int				right_fork;
	long			last_meal_time;
	int				meals_eaten;
	pthread_mutex_t	meal_mutex;
	t_table			*data;
}					t_philo;

void				assing_forks(t_philo *philo, int *first, int *last);
void				eat(t_table *data, t_philo *philo);
void				destroy_mutex(t_table *data, t_philo *philo);
int					init_philo(t_table *data, t_philo *philos);
void				init_mutex(t_table *data, t_philo *philo);
int					simulation_status(t_table *data);
int					meals_reached(t_table *data, t_philo *philo);
int					is_starving(t_table *data, t_philo philo);
void				death(t_table *data, t_philo philo);
void				*routine_1(void *arg);
void				*routine_2(void *arg);
int					is_numeric(char *s);
int					validate_input(int ac, char **av);
long				get_time(void);
long				timestamp_in_ms(t_table *data);
void				safe_print(t_table *data, char *msg, int id);

#endif
