/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_philo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiyamamo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 14:42:27 by hiyamamo          #+#    #+#             */
/*   Updated: 2022/10/08 14:42:29 by hiyamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_take_forks(t_philo *philo)
{
	struct timeval	t;

	philo->r_fork_locked = 1;
	pthread_mutex_lock(philo->r_fork);
	if (philo->set->is_dead == 1)
		return (1);
	gettimeofday(&t, NULL);
	printf("%ld %i has taken a fork\n", get_time_ms(&t), philo->id);
	philo->l_fork_locked = 1;
	pthread_mutex_lock(philo->l_fork);
	if (philo->set->is_dead == 1)
		return (1);
	gettimeofday(&t, NULL);
	printf("%ld %i has taken a fork\n", get_time_ms(&t), philo->id);
	return (0);
}

int	philo_eat(t_philo *philo)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	printf("%ld %i is eating\n", get_time_ms(&t), philo->id);
	philo->reset_start_t = 1;
	if (cust_usleep(philo, &t, philo->set->time_to_eat) == 1)
		return (1);
	pthread_mutex_unlock(philo->r_fork);
	philo->r_fork_locked = 0;
	pthread_mutex_unlock(philo->l_fork);
	philo->l_fork_locked = 0;
	return (0);
}

int	philo_sleep(t_philo *philo)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	printf("%ld %i is sleeping\n", get_time_ms(&t), philo->id);
	if (cust_usleep(philo, &t, philo->set->time_to_sleep) == 1)
		return (1);
	return (0);
}

void	philo_think(t_philo *philo)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	printf("%ld %i is thinking\n", get_time_ms(&t), philo->id);
}

void	*each_philo(void *arg)
{
	t_philo		*philo;
	pthread_t	deadcheck_tid;

	philo = arg;
	if (philo->id % 2 == 0)
		usleep(3000);
	pthread_create(&deadcheck_tid, NULL, philo_deadcheacker, philo);
	if (philo->set->is_dead == 1)
		return (aftertreat_thread(&deadcheck_tid));
	while (1)
	{
		if (philo_take_forks(philo) == 1)
			return (aftertreat_thread(&deadcheck_tid));
		if (philo_eat(philo) == 1)
			return (aftertreat_thread(&deadcheck_tid));
		if (philo_sleep(philo) == 1)
			return (aftertreat_thread(&deadcheck_tid));
		philo_think(philo);
	}
	return (NULL);
}
