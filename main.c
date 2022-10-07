#include "philo.h"

int	cust_usleep(t_philo *philo, struct timeval *t_start_act, int limit_ms)
{
	struct timeval	current_t;

	gettimeofday(&current_t, NULL);
	while (((current_t.tv_sec * 1000) + (current_t.tv_usec / 1000)) - \
			((t_start_act->tv_sec * 1000) + (t_start_act->tv_usec / 1000)) \
			 	< limit_ms)
	{
		if (((current_t.tv_sec * 1000) + (current_t.tv_usec / 1000)) - \
			((philo->start_t->tv_sec * 1000) + (philo->start_t->tv_usec / 1000)) \
				> philo->set->time_to_die || philo->set->is_dead == 1)
		{
			pthread_mutex_lock(philo->set->lock_is_dead);
			if (philo->set->is_dead != 1)
				printf("%ld: %i is dead\n", (current_t.tv_sec * 1000) + (current_t.tv_usec / 1000), philo->id);
			philo->set->is_dead = 1;
			pthread_mutex_unlock(philo->set->lock_is_dead);
			if (philo->r_fork_locked == 1)
				pthread_mutex_unlock(philo->r_fork);
			if (philo->l_fork_locked == 1)
				pthread_mutex_unlock(philo->l_fork);
			return (1);
		}
		usleep(3000);
		gettimeofday(&current_t, NULL);
	}
	return (0);
}

void	*is_philo_dead(void *arg)
{
	struct timeval	current_t;
	t_philo			*philo;

	philo = arg;
	while (1)
	{
		if (philo->reset_start_t == 1)
		{
			gettimeofday(philo->start_t, NULL);
			philo->reset_start_t = 0;
		}
		gettimeofday(&current_t, NULL);
		if (philo->set->is_dead == 1)
		{
			if (philo->r_fork_locked == 1)
				pthread_mutex_unlock(philo->r_fork);
			if (philo->l_fork_locked == 1)
				pthread_mutex_unlock(philo->l_fork);
			return (NULL);
		}
		if (((current_t.tv_sec * 1000) + (current_t.tv_usec / 1000)) - \
			((philo->start_t->tv_sec * 1000) + (philo->start_t->tv_usec / 1000)) \
				> philo->set->time_to_die)
		{
			pthread_mutex_lock(philo->set->lock_is_dead);
			if (philo->set->is_dead != 1)
				printf("%ld: %i is dead\n", (current_t.tv_sec * 1000) + (current_t.tv_usec / 1000), philo->id);
			philo->set->is_dead = 1;
			pthread_mutex_unlock(philo->set->lock_is_dead);
			if (philo->r_fork_locked == 1)
				pthread_mutex_unlock(philo->r_fork);
			if (philo->l_fork_locked == 1)
				pthread_mutex_unlock(philo->l_fork);
			return (NULL);
		}
		usleep(3000);
	}
}

void	*each_philo(void *arg)
{
	t_philo			*philo;
	struct timeval	t;
	pthread_t		deadcheck_tid;

	philo = arg;
	if (philo->id % 2 == 0)
		usleep(3000);
	pthread_create(&deadcheck_tid, NULL, is_philo_dead, philo);
	while (1)
	{
		// right fork
		philo->r_fork_locked = 1;
		pthread_mutex_lock(philo->r_fork);
		if (philo->set->is_dead == 1)
		{
			pthread_detach(deadcheck_tid);
			return (NULL);
		}
		gettimeofday(&t, NULL);
		printf("%ld: %i has taken a [right] fork\n", (t.tv_sec * 1000) + (t.tv_usec / 1000), philo->id);

		// left fork
		philo->l_fork_locked = 1;
		pthread_mutex_lock(philo->l_fork);
		if (philo->set->is_dead == 1)
		{
			pthread_detach(deadcheck_tid);
			return (NULL);
		}
		gettimeofday(&t, NULL);
		printf("%ld: %i has taken a [left] fork\n", (t.tv_sec * 1000) + (t.tv_usec / 1000), philo->id);
		
		// eat
		gettimeofday(&t, NULL);
		printf("%ld: %i is eating\n", (t.tv_sec * 1000) + (t.tv_usec / 1000), philo->id);
		philo->reset_start_t = 1;
		if (cust_usleep(philo, &t, philo->set->time_to_eat) == 1)
			return (NULL);
		pthread_mutex_unlock(philo->r_fork);
		philo->r_fork_locked = 0;
		pthread_mutex_unlock(philo->l_fork);
		philo->l_fork_locked = 0;

		// sleep
		gettimeofday(&t, NULL);
		printf("%ld: %i is sleeping\n", (t.tv_sec * 1000) + (t.tv_usec / 1000), philo->id);
		if (cust_usleep(philo, &t, philo->set->time_to_sleep) == 1)
			return (NULL);

		// think
		gettimeofday(&t, NULL);
		printf("%ld: %i is thinking\n", (t.tv_sec * 1000) + (t.tv_usec / 1000), philo->id);
	}
	return (NULL);
}

int	main(int argc, char *argv[], char *envp[])
{
	int				num_of_philo;
	pthread_mutex_t	*fork[10]; //TODO: check
	pthread_mutex_t	*lock_is_dead;
	t_philo			*philo;
	int				i;
	t_set			set;
	struct timeval	*start_t;

	if (argc != 5)
	{
		printf("arg error\n");
		return (1);
	}
	num_of_philo = ft_atoi(argv[1]);
	set.time_to_die = ft_atoi(argv[2]);
	set.time_to_eat = ft_atoi(argv[3]);
	set.time_to_sleep = ft_atoi(argv[4]);
	lock_is_dead = (pthread_mutex_t *) malloc (sizeof (pthread_mutex_t));
	pthread_mutex_init(lock_is_dead, NULL);
	set.lock_is_dead = lock_is_dead;
	set.is_dead = 0;
	i = 0;
	while (i++ < num_of_philo)
	{
		fork[i] = (pthread_mutex_t *) malloc (sizeof (pthread_mutex_t));
		pthread_mutex_init(fork[i], NULL);
	}
	i = 0;
	while (i++ < num_of_philo)
	{
		philo = (t_philo *) malloc (sizeof (t_philo));
		philo->set = &set;
		philo->id = i;
		philo->r_fork = fork[i];
		philo->l_fork = fork[i % num_of_philo + 1];
		philo->r_fork_locked = 0;
		philo->l_fork_locked = 0;
		philo->tid = (pthread_t *) malloc (sizeof (pthread_t));
		start_t = (struct timeval *) malloc (sizeof (struct timeval));
		gettimeofday(start_t, NULL);
		philo->start_t = start_t;
		philo->reset_start_t = 0;
		pthread_create(philo->tid, NULL, each_philo, philo);
	}
	i = 0;
	while (i++ < num_of_philo)
	{
		pthread_join(*(philo->tid), NULL);
		pthread_mutex_destroy(fork[i]);
	}
	return (0);
}
