/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_philo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiyamamo <hiyamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 14:42:27 by hiyamamo          #+#    #+#             */
/*   Updated: 2022/10/10 21:07:27 by hiyamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_take_forks(t_philo *philo)
{
	struct timeval	t;

	pthread_mutex_lock(philo->r_fork);
	gettimeofday(&t, NULL);
	if (check_is_dead_and_printf(philo, "has taken a fork", \
		get_time_ms(&t)) == 1)
		return (1);
	pthread_mutex_lock(philo->l_fork);
	gettimeofday(&t, NULL);
	if (check_is_dead(philo) == 0)
		printf("%ld %i has taken a fork\n", get_time_ms(&t), philo->id);
	else
		return (1);
	return (0);
}

int	philo_eat(t_philo *philo)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	if (check_is_dead_and_printf(philo, "is eating", get_time_ms(&t)) == 1)
		return (1);
	gettimeofday(philo->start_t, NULL);
	if (cust_usleep(philo, &t, philo->set->time_to_eat) == 1)
		return (1);
	pthread_mutex_unlock(philo->r_fork);
	pthread_mutex_unlock(philo->l_fork);
	return (0);
}

int	philo_sleep(t_philo *philo)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	if (check_is_dead_and_printf(philo, "is sleeping", get_time_ms(&t)) == 1)
		return (1);
	if (cust_usleep(philo, &t, philo->set->time_to_sleep) == 1)
		return (1);
	return (0);
}

void	philo_think(t_philo *philo)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	check_is_dead_and_printf(philo, "is thinking", get_time_ms(&t));
}

void	*each_philo(void *arg)
{
	t_philo		*philo;

	philo = arg;
	if (philo->id % 2 == 0)
		usleep(3000);
	if (check_is_dead(philo) == 1)
		return (NULL);
	while (1)
	{
		if (philo_take_forks(philo) == 1)
			return (NULL);
		if (philo_eat(philo) == 1)
			return (NULL);
		if (philo_sleep(philo) == 1)
			return (NULL);
		philo_think(philo);
	}
	return (NULL);
}
