/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 21:30:52 by jorgonca          #+#    #+#             */
/*   Updated: 2024/06/20 22:38:25 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_other_mutexes(t_data *data, int i)
{
	int	j;

	j = 0;
	if (initialize_mutex(&data->print_lock) == 0)
		j++;
	if (j == 1 && initialize_mutex(&data->death) == 0)
		j++;
	if (j == 2 && initialize_mutex(&data->dined) == 0)
		j++;
	if (j == 3 && initialize_mutex(&data->meals_eaten_mutex) == 0)
		j++;
	if (initialize_mutex(&data->last_meal_timestamps_mutex) == 0)
		j++;
	if (j == 5)
		return (0);
	if (j >= 1)
		pthread_mutex_destroy(&data->print_lock);
	if (j >= 2)
		pthread_mutex_destroy(&data->death);
	if (j >= 3)
		pthread_mutex_destroy(&data->dined);
	if (j >= 4)
		pthread_mutex_destroy(&data->meals_eaten_mutex);
	return (destroy_fork_mutexes(data, i));
}

int	initialize_mutexes(t_data *data)
{
	int	i;

	i = 0;
	data->fork = (pthread_mutex_t *)ft_calloc(data->number_of_philosophers,
			sizeof(pthread_mutex_t));
	if (data->fork == NULL)
	{
		free(data->philosophers);
		free(data);
		return (1);
	}
	while (i < data->number_of_philosophers)
	{
		if (initialize_mutex(&data->fork[i]) == 1)
			return (destroy_fork_mutexes(data, i));
		i++;
	}
	if (init_other_mutexes(data, i) == 1)
		return (1);
	return (0);
}

int	initialize_philosopher_data(t_philosopher *philosopher, int id,
		long start_time, t_data *data)
{
	philosopher->id = id;
	philosopher->last_meal_time = get_current_time();
	if (philosopher->last_meal_time == -1)
	{
		destroy_mutexes(data);
		return (1);
	}
	philosopher->meals_eaten = 0;
	philosopher->time_to_die = data->time_to_die;
	philosopher->data = data;
	philosopher->data->death_note = 0;
	philosopher->data->dined_enough = 0;
	pthread_mutex_lock(&data->last_meal_timestamps_mutex);
	data->last_meal_timestamps[id - 1] = start_time;
	pthread_mutex_unlock(&data->last_meal_timestamps_mutex);
	return (0);
}

int	init_multiple_philosophers(t_philosopher *philosophers, t_data *data)
{
	int		i;
	long	start_time;

	start_time = get_current_time();
	if (start_time == -1)
	{
		destroy_mutexes(data);
		return (1);
	}
	i = 0;
	while (i < data->number_of_philosophers)
	{
		data->start_time = start_time;
		if (initialize_philosopher_data(&philosophers[i], i + 1, start_time,
				data) == 1)
			return (1);
		assign_forks(&philosophers[i], data);
		i++;
	}
	return (0);
}

int	initialize_philosophers(t_data *data)
{
	t_philosopher	*philosophers;

	philosophers = allocate_philosophers(data->number_of_philosophers);
	if (!philosophers)
	{
		free(data);
		return (1);
	}
	data->philosophers = philosophers;
	if (initialize_mutexes(data) != 0)
	{
		write(2, "Error: initialize_mutexes\n", 25);
		return (1);
	}
	if (init_multiple_philosophers(philosophers, data) == 1)
		return (1);
	return (0);
}
