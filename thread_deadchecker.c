/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_deadchecker.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiyamamo <hiyamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 14:42:38 by hiyamamo          #+#    #+#             */
/*   Updated: 2022/10/10 20:51:22 by hiyamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	handle_dead(t_philo *philo, struct timeval *current_t)
{
	pthread_mutex_lock(philo->set->lock_is_dead);
	if (philo->set->is_dead != 1)
	{
		printf("%ld %i died\n", get_time_ms(current_t), philo->id);
		philo->set->is_dead = 1;
	}
	pthread_mutex_unlock(philo->set->lock_is_dead);
	aftertreat_mutex(philo);
}

void	*philo_deadcheacker(void *arg)
{
	struct timeval	current_t;
	t_philo			*philo;

	philo = arg;
	while (1)
	{
		gettimeofday(&current_t, NULL);
		if (check_is_dead(philo) == 1)
		{
			aftertreat_mutex(philo);
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
