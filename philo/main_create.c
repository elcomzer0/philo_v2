/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_create.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 21:32:35 by jorgonca          #+#    #+#             */
/*   Updated: 2024/06/20 22:28:33 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	create_monitor_threads(t_data *data)
{
	if (pthread_create(&(data->monitor_eat), NULL, monitor_eat, data) != 0)
	{
		write(2, "Error creating monitor thread\n", 30);
		return (-1);
	}
	if (pthread_create(&(data->monitor_death), NULL, monitor_death, data) != 0)
	{
		write(2, "Error creating monitor thread\n", 30);
		return (-1);
	}
	return (0);
}

void	destroy_mutex_in_create_thread(t_data *data)
{
	pthread_mutex_destroy(&data->print_lock);
	pthread_mutex_destroy(&data->death);
	pthread_mutex_destroy(&data->dined);
	pthread_mutex_destroy(&data->meals_eaten_mutex);
	destroy_fork_mutexes(data, data->number_of_philosophers);
}

void	join_threads_failure(t_data *data, int i)
{
	int	j;

	j = 0;
	while (j <= i && data->philosophers[j].thread)
	{
		pthread_join(data->philosophers[j].thread, NULL);
		j++;
	}
	if (data->monitor_eat)
		pthread_join(data->monitor_eat, NULL);
	if (data->monitor_death)
		pthread_join(data->monitor_death, NULL);
}

t_data	*create_threads(t_data *data)
{
	int	i;

	i = 0;
	if (create_monitor_threads(data) != 0)
		data->all_philosophers_created = 1;
	while (data->all_philosophers_created != 1
		&& i < data->number_of_philosophers)
	{
		if (pthread_create(&(data->philosophers[i].thread), NULL,
				philosopher_routine, &(data->philosophers[i])) != 0)
			data->all_philosophers_created = 1;
		i++;
	}
	if (data->all_philosophers_created == 1)
	{
		join_threads_failure(data, i);
		destroy_mutex_in_create_thread(data);
		data = NULL;
		return (data);
	}
	return (data);
}
