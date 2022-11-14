/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiyamamo <hiyamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 15:36:42 by hiyamamo          #+#    #+#             */
/*   Updated: 2022/10/08 15:39:19 by hiyamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_arg(int argc, char **argv)
{
	int	i;
	int	k;

	if (argc > 6 || argv[1][0] == '0')
	{
		printf("Error with arguments\n");
		return (1);
	}
	i = 1;
	while (i < argc)
	{
		k = 0;
		while (argv[i][k] != '\0')
		{
			if (!(47 < argv[i][k] && argv[i][k] < 58))
			{
				printf("Error with arguments\n");
				return (1);
			}
			k++;
		}
		i++;
	}
	return (0);
}

int	ft_atoi(const char *str)
{
	int		i;
	int		sign;
	size_t	res;

	i = 0;
	sign = 1;
	res = 0;
	while (str[i] == ' ' || str[i] == '\n' || str[i] == '\t' \
			|| str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i] == '-' || str[i] == '+')
		if (str[i++] == '-')
			sign = -1;
	while ('0' <= str[i] && str[i] <= '9' && str[i] != '\0')
	{
		if (res > LONG_MAX / 10 && sign < 0)
			return (0);
		if (res > LONG_MAX / 10 && sign > 0)
			return (-1);
		res = res * 10 + str[i++] - '0';
	}
	return ((int)(res * sign));
}
