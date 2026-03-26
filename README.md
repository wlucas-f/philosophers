*This project has been created as part of the 42 curriculum by wlucas-f*

## Description

The "Philosophers" project introduces the basics of threading a process. The primary goal is to learn how to create threads and explore the use of mutexes.

The program simulates the classic Dining Philosophers problem, where one or more philosophers sit at a round table with a large bowl of spaghetti in the middle.

- The philosophers take turns eating, thinking, and sleeping.

- While they are eating, they are not thinking nor sleeping.

- While thinking, they are not eating nor sleeping.

- While sleeping, they are not eating nor thinking.

- There are as many forks on the table as there are philosophers.

- A philosopher must pick up both the fork to their right and the fork to their left before eating.

- When a philosopher has finished eating, they put their forks back on the table and start sleeping.

- Once awake, they start thinking again.

- The simulation stops when a philosopher dies of starvation.

- In the mandatory part, each philosopher must be represented as a separate thread. To prevent duplicating forks, each fork's state is protected with a mutex.

## Instructions

### Compilation

- Compile the source files using the provided Makefile.

- The Makefile contains the required rules: $(NAME), all, clean, fclean, and re.


### Run

```
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]
```

#### Examples

```bash
# 5 philosophers, none should die
./philo 5 800 200 200

# 4 philosophers, each must eat at least 3 times
./philo 4 800 200 200 3

# Edge case: 1 philosopher (will die – only one fork available)
./philo 1 800 200 200
```

## Resources

- [Dining Philosophers problem – Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [Unix Threads in C - Youtube](https://www.youtube.com/watch?v=d9s_d28yJq0&list=PLfqABt5AS4FmuQf70psXrsMLEDQXNkLq2)
- [POSIX Threads Programming – LLNL](https://hpc-tutorials.llnl.gov/posix/)


### AI Usage
- README.
