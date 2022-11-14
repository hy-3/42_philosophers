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

int	check_is_everyone_full(int num_of_full_philo, t_set *set)
{
	if (num_of_full_philo == set->num_of_philo)
	{
		pthread_mutex_lock(set->lock_is_dead);
		if (set->is_dead != 1)
			set->is_dead = 1;
		pthread_mutex_unlock(set->lock_is_dead);
		return (1);
	}
	return (0);
}

void	checker(t_set *set, t_philo **philo)
{
	struct timeval	current_t;
	int				i;
	int				num_of_full_philo;

	while (1)
	{
		gettimeofday(&current_t, NULL);
		i = 0;
		num_of_full_philo = 0;
		while (i++ < set->num_of_philo)
		{
			if (check_is_dead(philo[i]) == 1)
				aftertreat_mutex(philo[i]);
			if (get_time_ms(&current_t) - get_time_ms(philo[i]->start_t) \
				> philo[i]->set->time_to_die)
				handle_dead(philo[i], &current_t);
			if (set->max_eat >= 0 && philo[i]->num_eat >= set->max_eat)
				num_of_full_philo++;
		}
		if (check_is_dead(philo[1]) == 1 || \
			check_is_everyone_full(num_of_full_philo, set) == 1)
			return ;
		usleep(USLEEP_TIME);
	}
}
