/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiyamamo <hiyamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/19 17:04:35 by hiyamamo          #+#    #+#             */
/*   Updated: 2022/09/19 19:14:00 by hiyamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <pthread.h>

typedef struct s_philo
{
	int				id;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*r_fork;
}	t_philo;

int	ft_atoi(const char *str);

#endif