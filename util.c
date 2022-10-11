/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiyamamo <hiyamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 14:42:44 by hiyamamo          #+#    #+#             */
/*   Updated: 2022/10/10 21:01:51 by hiyamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	aftertreat_mutex(t_philo *philo)
{
	pthread_mutex_unlock(philo->r_fork);
	pthread_mutex_unlock(philo->l_fork);
}

int	check_is_dead_and_printf(t_philo *philo, char *str, long ms)
{
	int	res;

	res = 0;
	pthread_mutex_lock(philo->set->lock_is_dead);
	if (philo->set->is_dead == 0)
		printf("%ld %i %s\n", ms, philo->id, str);
	else
		res = 1;
	pthread_mutex_unlock(philo->set->lock_is_dead);
	return (res);
}

int	check_is_dead(t_philo *philo)
{
	int	res;

	res = 0;
	pthread_mutex_lock(philo->set->lock_is_dead);
	if (philo->set->is_dead == 1)
		res = 1;
	pthread_mutex_unlock(philo->set->lock_is_dead);
	return (res);
}

int	cust_usleep(t_philo *philo, struct timeval *t_start_act, int limit_ms)
{
	struct timeval	current_t;

	gettimeofday(&current_t, NULL);
	while (get_time_ms(&current_t) - get_time_ms(t_start_act) < limit_ms)
	{
		if (get_time_ms(&current_t) - get_time_ms(philo->start_t) \
			> philo->set->time_to_die || check_is_dead(philo) == 1)
		{
			pthread_mutex_lock(philo->set->lock_is_dead);
			if (philo->set->is_dead != 1)
			{
				printf("%ld %i died\n", get_time_ms(&current_t), philo->id);
				philo->set->is_dead = 1;
			}
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
