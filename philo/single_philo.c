/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_philo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 10:05:31 by jorgonca          #+#    #+#             */
/*   Updated: 2024/06/19 15:55:12 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* void	*sokrates_case(void *arg)
{
	t_data	*data;
	t_philosopher	*philo;
	data = (t_data *)arg;
	
	usleep((int)data->time_to_die);
	usleep((int)data->time_to_eat);
	printf("%ld 1 has taken a fork\n", data->time_to_eat);
	printf("%ld 1 died\n", data->time_to_die);
	return (NULL);
} */

void	*sokrates_case(void *arg)
{
	t_data	*data;
	t_philosopher	*philo;

	data = (t_data *)arg;
	philo = data->philosophers;
	printf("ahllo");
	printf("philo->id: %d\n", philo->id);
	ft_usleep(data->start_time, philo);
	ft_usleep(data->time_to_die, philo);
	printf("time to die: %ld\n", data->time_to_die);
	print_status(philo, "has taken a fork");
	ft_usleep(data->time_to_sleep, philo);	
	print_status(philo, "is sleeping");
	print_status(philo, "is thinking");
	print_status(philo, "died");
	printf("philo->time_to_die: %ld\n", philo->time_to_die);
	
	/* usleep((int)data->time_to_die);
	usleep((int)data->time_to_eat);
	printf("%ld 1 has taken a fork\n", data->time_to_eat);
	printf("%ld 1 died\n", data->time_to_die); */
	return (NULL);
}

void	create_single_philosopher_thread(t_data *data)
{
	pthread_t	thread;

	if (pthread_create(&thread, NULL, &sokrates_case, data) != 0)
	{
		write(STDERR_FILENO, "Error creating philosopher thread\n", 35);
		return ;
	}
	pthread_join(thread, NULL);
}

void	initialize_single_philosopher_case(t_data *data)
{
	long start_time;
	//int id;
	
	start_time = get_current_time();
	data->start_time = start_time;
	data->philosophers->id = 1;
	data->philosophers->time_to_die = data->time_to_die;
	create_single_philosopher_thread(data);
}

// one phillo
// takes fork
// usleep (2 x ttd)
// died
