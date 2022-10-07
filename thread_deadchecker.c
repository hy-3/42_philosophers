#include "philo.h"

void	reset_philo_start_t(t_philo *philo)
{
	gettimeofday(philo->start_t, NULL);
	philo->reset_start_t = 0;
}

void	handle_dead(t_philo *philo, struct timeval *current_t)
{
	pthread_mutex_lock(philo->set->lock_is_dead);
	if (philo->set->is_dead != 1)
		printf("%ld %i died\n", get_time_ms(current_t), philo->id);
	philo->set->is_dead = 1;
	pthread_mutex_unlock(philo->set->lock_is_dead);
	aftertreat_dead(philo);
}

void	*philo_deadcheacker(void *arg)
{
	struct timeval	current_t;
	t_philo			*philo;

	philo = arg;
	while (1)
	{
		if (philo->reset_start_t == 1)
			reset_philo_start_t(philo);
		gettimeofday(&current_t, NULL);
		if (philo->set->is_dead == 1)
		{
			aftertreat_dead(philo);
			return (NULL);
		}
		if (get_time_ms(&current_t) - get_time_ms(philo->start_t) \
			> philo->set->time_to_die)
		{
			handle_dead(philo, &current_t);
			return (NULL);
		}
		usleep(3000);
	}
}
