/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiyamamo <hiyamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 14:42:06 by hiyamamo          #+#    #+#             */
/*   Updated: 2022/10/08 15:39:30 by hiyamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	initialize_and_run_philos(t_set *set, t_philo **philo)
{
	int				i;
	struct timeval	*start_t;

	i = 0;
	while (i++ < set->num_of_philo)
	{
		philo[i] = (t_philo *) malloc (sizeof(t_philo));
		philo[i]->set = set;
		philo[i]->id = i;
		philo[i]->r_fork = set->fork[i];
		philo[i]->l_fork = set->fork[i % set->num_of_philo + 1];
		philo[i]->r_fork_locked = 0;
		philo[i]->l_fork_locked = 0;
		philo[i]->tid = (pthread_t *) malloc (sizeof (pthread_t));
		start_t = (struct timeval *) malloc (sizeof (struct timeval));
		gettimeofday(start_t, NULL);
		philo[i]->start_t = start_t;
		philo[i]->reset_start_t = 0;
		pthread_create(philo[i]->tid, NULL, each_philo, philo[i]);
	}
}

void	join_lastphilo_and_free(t_set *set, t_philo **philo)
{
	int	i;
	int	num;

	i = 0;
	num = set->num_of_philo;
	pthread_join(*(philo[num]->tid), NULL);
	pthread_mutex_destroy(set->lock_is_dead);
	free(set->lock_is_dead);
	while (i++ < num)
	{
		free(set->fork[i]);
		if (i != num)
			pthread_detach(*(philo[i]->tid));
		free(philo[i]->tid);
		free(philo[i]->start_t);
		free(philo[i]);
	}
}

int	main(int argc, char *argv[])
{
	t_set	set;
	t_philo	*philo[500];

	if (check_arg(argc, argv) == 1)
		return (1);
	initialize_set(&set, argv);
	initialize_fork(&set);
	initialize_and_run_philos(&set, philo);
	join_lastphilo_and_free(&set, philo);
	return (0);
}
