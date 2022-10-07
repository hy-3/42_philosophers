/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiyamamo <hiyamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/19 17:04:35 by hiyamamo          #+#    #+#             */
/*   Updated: 2022/10/03 19:13:11 by hiyamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <errno.h>

typedef struct s_set
{
	int				num_of_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	pthread_mutex_t	*lock_is_dead;
	int				is_dead;
	pthread_mutex_t	*fork[500];
}	t_set;

typedef struct s_philo
{
	pthread_t		*tid;
	int				id;
	t_set			*set;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*l_fork;
	int				r_fork_locked;
	int				l_fork_locked;
	struct timeval	*start_t;
	int				reset_start_t;
}	t_philo;

// thread_philo.c
void	*each_philo(void *arg);

// thread_deadchecker.c
void	*philo_deadcheacker(void *arg);

// util.c
void	aftertreat_mutex(t_philo *philo);
void	*aftertreat_thread(t_philo *philo, pthread_t *deadcheck_tid);
int		cust_usleep(t_philo *philo, struct timeval *t_start_act, int limit_ms);
long	get_time_ms(struct timeval *t);
int		ft_atoi(const char *str);

#endif