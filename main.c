#include "philo.h"

void	*each_philo(void *arg)
{
	t_philo	*philo;

	philo = arg;
	if (pthread_mutex_lock(philo->l_fork) == -1)
		pthread_mutex_unlock(philo->l_fork);
	if (pthread_mutex_lock(philo->r_fork) == -1)
	{
		pthread_mutex_unlock(philo->l_fork);
		pthread_mutex_unlock(philo->r_fork);
	}
	printf("%i is eating\n", philo->id);
	usleep(5000);

	pthread_mutex_unlock(philo->l_fork);
	pthread_mutex_unlock(philo->r_fork);
	return (NULL);
}

int	main(int argc, char *argv[], char *envp[])
{
	int				num_of_philo;
	pthread_t		*tid[10];
	pthread_mutex_t	*fork[10]; //TODO: check
	t_philo			*philo[10];
	int				i;

	if (argc != 5)
	{
		printf("arg error\n");
		return (1);
	}
	num_of_philo = ft_atoi(argv[1]);
	i = 0;
	while (i++ < num_of_philo)
	{
		philo[i] = (t_philo *) malloc (sizeof (t_philo));
		fork[i] = (pthread_mutex_t *) malloc (sizeof (pthread_mutex_t));
		pthread_mutex_init(fork[i], NULL);
	}
	philo[1]->id = 1;
	philo[1]->l_fork = fork[1];
	if (num_of_philo != 1)
		philo[1]->r_fork = fork[num_of_philo];
	tid[1] = (pthread_t *) malloc (sizeof (pthread_t));
	pthread_create(tid[1], NULL, each_philo, philo[1]);
	i = 1;
	while (i++ < num_of_philo)
	{
		philo[i]->id = i;
		philo[i]->l_fork = fork[i - 1];
		philo[i]->r_fork = fork[i];
		tid[i] = (pthread_t *) malloc (sizeof (pthread_t));
		pthread_create(tid[i], NULL, each_philo, philo[i]);
	}
	i = 0;
	while (i++ < num_of_philo)
	{
		pthread_join(*tid[i], NULL);
		pthread_mutex_destroy(fork[i]);
	}
	return (0);
}
