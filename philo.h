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
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	pthread_mutex_t	*lock_is_dead;
	int				is_dead;
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

int	ft_atoi(const char *str);

#endif