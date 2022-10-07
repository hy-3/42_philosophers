#include "philo.h"

void	aftertreat_mutex(t_philo *philo)
{
	if (philo->r_fork_locked == 1)
		pthread_mutex_unlock(philo->r_fork);
	if (philo->l_fork_locked == 1)
		pthread_mutex_unlock(philo->l_fork);
	pthread_mutex_destroy(philo->set->fork[philo->id]);
}

void	*aftertreat_thread(pthread_t *deadcheck_tid)
{
	pthread_detach(*deadcheck_tid);
	return (NULL);
}

int	cust_usleep(t_philo *philo, struct timeval *t_start_act, int limit_ms)
{
	struct timeval	current_t;

	gettimeofday(&current_t, NULL);
	while (get_time_ms(&current_t) - get_time_ms(t_start_act) < limit_ms)
	{
		if (get_time_ms(&current_t) - get_time_ms(philo->start_t) \
			> philo->set->time_to_die || philo->set->is_dead == 1)
		{
			pthread_mutex_lock(philo->set->lock_is_dead);
			if (philo->set->is_dead != 1)
				printf("%ld %i died\n", get_time_ms(&current_t), philo->id);
			philo->set->is_dead = 1;
			pthread_mutex_unlock(philo->set->lock_is_dead);
			aftertreat_mutex(philo);
			return (1);
		}
		usleep(3000);
		gettimeofday(&current_t, NULL);
	}
	return (0);
}

long	get_time_ms(struct timeval *t)
{
	long	timestamp_ms;

	timestamp_ms = (t->tv_sec * 1000) + (t->tv_usec / 1000);
	return (timestamp_ms);
}

int	ft_atoi(const char *str)
{
	int		i;
	int		sign;
	size_t	res;

	i = 0;
	sign = 1;
	res = 0;
	while (str[i] == ' ' || str[i] == '\n' || str[i] == '\t' \
			|| str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i] == '-' || str[i] == '+')
		if (str[i++] == '-')
			sign = -1;
	while ('0' <= str[i] && str[i] <= '9' && str[i] != '\0')
	{
		if (res > LONG_MAX / 10 && sign < 0)
			return (0);
		if (res > LONG_MAX / 10 && sign > 0)
			return (-1);
		res = res * 10 + str[i++] - '0';
	}
	return ((int)(res * sign));
}
