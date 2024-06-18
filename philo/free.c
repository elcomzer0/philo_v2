/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 21:53:55 by jorgonca          #+#    #+#             */
/*   Updated: 2024/06/19 00:05:34 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_resources(t_data *data)
{
	if (data->fork)
	{
		free(data->fork);
		data->fork = NULL;
	}
	if (data->philosophers)
	{
		free(data->philosophers);
		data->philosophers = NULL;
	}
	if (data)
	{
		free(data);
		data = NULL;
	}
}

void	destroy_mutexes(t_data *data)
{
	if (pthread_mutex_destroy(&data->last_meal_timestamps_mutex) != 0)
		write(2, "Error: last_meal_timestamps mutex_destroy\n", 43);
	if (pthread_mutex_destroy(&data->print_lock) != 0)
		write(2, "Error: print mutex_destroy\n", 28);
	if (pthread_mutex_destroy(&data->death) != 0)
		write(2, "Error: death mutex_destroy\n", 27);
	if (pthread_mutex_destroy(&data->dined) != 0)
		write(2, "Error: dined mutex_destroy\n", 27);
	if (pthread_mutex_destroy(&data->dining_mutex) != 0)
		write(2, "Error: dining mutex_destroy\n", 27);
}

void	join_threads(t_data *data)
{
	int	i;

	if (data->number_of_philosophers > 1)
	{
		i = 0;
		while (i < data->number_of_philosophers)
		{
			pthread_join(data->philosophers[i].thread, NULL);
			i++;
		}
	}
	if (data->monitor_eat)
		pthread_join(data->monitor_eat, NULL);
	if (data->monitor_death)
		pthread_join(data->monitor_death, NULL);
}

void	clean_exit(t_data *data)
{
	if (!data)
		return ;
	join_threads(data);
	destroy_mutexes(data);
	free_resources(data);
}
