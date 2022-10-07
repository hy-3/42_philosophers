#include "philo.h"

void	initialize_set(t_set *set, char **argv)
{
	pthread_mutex_t	*lock_is_dead;

	set->num_of_philo = ft_atoi(argv[1]);
	set->time_to_die = ft_atoi(argv[2]);
	set->time_to_eat = ft_atoi(argv[3]);
	set->time_to_sleep = ft_atoi(argv[4]);
	lock_is_dead = (pthread_mutex_t *) malloc (sizeof (pthread_mutex_t));
	pthread_mutex_init(lock_is_dead, NULL);
	set->lock_is_dead = lock_is_dead;
	set->is_dead = 0;
}

void	initialize_fork(t_set *set)
{
	int	i;

	i = 0;
	while (i++ < set->num_of_philo)
	{
		set->fork[i] = (pthread_mutex_t *) malloc (sizeof (pthread_mutex_t));
		pthread_mutex_init(set->fork[i], NULL);
	}
}

t_philo	*initialize_and_run_philos(t_set *set)
{
	int				i;
	t_philo			*philo;
	struct timeval	*start_t;

	i = 0;
	while (i++ < set->num_of_philo)
	{
		philo = (t_philo *) malloc (sizeof (t_philo));
		philo->set = set;
		philo->id = i;
		philo->r_fork = set->fork[i];
		philo->l_fork = set->fork[i % set->num_of_philo + 1];
		philo->r_fork_locked = 0;
		philo->l_fork_locked = 0;
		philo->tid = (pthread_t *) malloc (sizeof (pthread_t));
		start_t = (struct timeval *) malloc (sizeof (struct timeval));
		gettimeofday(start_t, NULL);
		philo->start_t = start_t;
		philo->reset_start_t = 0;
		pthread_create(philo->tid, NULL, each_philo, philo);
	}
	return (philo);
}

void	wait_last_philo(t_philo *philo)
{
	int	i;

	i = 0;
	pthread_join(*(philo->tid), NULL);
}

int	main(int argc, char *argv[])
{
	t_set	set;
	t_philo	*philo;

	if (argc != 5)
	{
		printf("arg error\n");
		return (1);
	}
	initialize_set(&set, argv);
	initialize_fork(&set);
	philo = initialize_and_run_philos(&set);
	wait_last_philo(philo);
	return (0);
}
