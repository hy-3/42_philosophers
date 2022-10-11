/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiyamamo <hiyamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/19 17:04:35 by hiyamamo          #+#    #+#             */
/*   Updated: 2022/10/08 15:40:33 by hiyamamo         ###   ########.fr       */
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
	struct timeval	*start_t;
}	t_philo;

// deadchecker.c
void	philos_deadchecker(t_set *set, t_philo **philo);

// thread_philo.c
void	*each_philo(void *arg);

// util.c
void	aftertreat_mutex(t_philo *philo);
int		cust_usleep(t_philo *philo, struct timeval *t_start_act, int limit_ms);
long	get_time_ms(struct timeval *t);
int		check_is_dead(t_philo *philo);
int		check_is_dead_and_printf(t_philo *philo, char *str, long ms);

// util2.c
int		check_arg(int argc, char **argv);
int		ft_atoi(const char *str);

#endif