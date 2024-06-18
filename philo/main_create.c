/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_create.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 21:32:35 by jorgonca          #+#    #+#             */
/*   Updated: 2024/06/18 21:49:47 by jorgonca         ###   ########.fr       */
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

int	create_threads(t_data *data)
{
	int i;
	int j;

	i = 0;
	while (i < data->number_of_philosophers)
	{
		if (pthread_create(&(data->philosophers[i].thread), NULL,
				philosopher_routine, &(data->philosophers[i])) != 0)
		{
			write(2, "Error creating philosopher thread\n", 31);
			j = 0;
			while (j <= i)
			{
				pthread_mutex_destroy(&(data)->fork[j]);
				j++;
			}
			pthread_mutex_destroy(&data->print_lock);
			return (-1);
		}
		usleep(100);
		i++;
	}
	if (create_monitor_threads(data) != 0)
		return (-1);
	return (0);
}