#include "philo.h"

void	*is_philo_dead(void *arg)
{
	struct timeval	start_t;
	struct timeval	current_t;
	t_philo			*philo;

	philo = arg;
	gettimeofday(&start_t, NULL);
	while (1)
	{
		gettimeofday(&current_t, NULL);
		if (((current_t.tv_sec * 1000) + (current_t.tv_usec / 1000)) - \
			((start_t.tv_sec * 1000) + (start_t.tv_usec / 1000)) \
				> philo->set->time_to_die)
		{
			printf("%ld: %i is dead\n", (current_t.tv_sec * 1000) + (current_t.tv_usec / 1000), philo->id);
			pthread_detach(*(philo->tid));
			philo->is_dead = 1;
			return (NULL);
		}
		if (philo->reset == 1)
		{
			philo->reset = 0;
			return (NULL);
		}
		usleep(5000);
	}
}

void	*each_philo(void *arg)
{
	t_philo			*philo;
	struct timeval	t;
	pthread_t		deadcheck_tid;

	philo = arg;
	if (philo->id % 2 == 0)
		usleep(5000);
	while (1)
	{
		pthread_create(&deadcheck_tid, NULL, is_philo_dead, philo); //TODO: check from last meal
		gettimeofday(&t, NULL);
		printf("%ld: %i is thinking\n", (t.tv_sec * 1000) + (t.tv_usec / 1000), philo->id);

		pthread_mutex_lock(philo->r_fork);
		if (philo->is_dead == 1)
			return (NULL);
		gettimeofday(&t, NULL);
		printf("%ld: %i has taken a [right] fork\n", (t.tv_sec * 1000) + (t.tv_usec / 1000), philo->id);

		pthread_mutex_lock(philo->l_fork);
		if (philo->is_dead == 1)
			return (NULL);
		gettimeofday(&t, NULL);
		printf("%ld: %i has taken a [left] fork\n", (t.tv_sec * 1000) + (t.tv_usec / 1000), philo->id);
		
		gettimeofday(&t, NULL);
		philo->reset = 1;
		pthread_join(deadcheck_tid, NULL);
		printf("%ld: %i is eating\n", (t.tv_sec * 1000) + (t.tv_usec / 1000), philo->id);
		usleep(philo->set->time_to_eat * 1000);
		
		pthread_mutex_unlock(philo->r_fork);
		pthread_mutex_unlock(philo->l_fork);

		gettimeofday(&t, NULL);
		printf("%ld: %i is sleeping\n", (t.tv_sec * 1000) + (t.tv_usec / 1000), philo->id);
		usleep(philo->set->time_to_sleep * 1000);
	}
	return (NULL);
}

int	main(int argc, char *argv[], char *envp[])
{
	int				num_of_philo;
	pthread_mutex_t	*fork[10]; //TODO: check
	t_philo			*philo[10];
	int				i;
	t_set			set;

	if (argc != 5)
	{
		printf("arg error\n");
		return (1);
	}
	num_of_philo = ft_atoi(argv[1]);
	set.time_to_die = ft_atoi(argv[2]);
	set.time_to_eat = ft_atoi(argv[3]);
	set.time_to_sleep = ft_atoi(argv[4]);
	i = 0;
	while (i++ < num_of_philo)
	{
		philo[i] = (t_philo *) malloc (sizeof (t_philo));
		philo[i]->set = &set;
		fork[i] = (pthread_mutex_t *) malloc (sizeof (pthread_mutex_t));
		pthread_mutex_init(fork[i], NULL);
	}
	i = 0;
	while (i++ < num_of_philo)
	{
		philo[i]->id = i;
		philo[i]->r_fork = fork[i];
		philo[i]->l_fork = fork[i + 1 % num_of_philo];
		philo[i]->reset = 0;
		philo[i]->is_dead = 0;
		philo[i]->tid = (pthread_t *) malloc (sizeof (pthread_t));
		pthread_create(philo[i]->tid, NULL, each_philo, philo[i]);
	}
	i = 0;
	while (i++ < num_of_philo)
	{
		pthread_join(*(philo[i]->tid), NULL);
		pthread_mutex_destroy(fork[i]);
	}
	return (0);
}
