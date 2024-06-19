/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 21:30:52 by jorgonca          #+#    #+#             */
/*   Updated: 2024/06/19 18:16:22 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	initialize_mutex(pthread_mutex_t *mutex, t_data *data)
{
	int	j;

	if (pthread_mutex_init(mutex, NULL) != 0)
	{
		write(2, "Error: mutex_init\n", 18);
		j = 0;
		while (j < data->number_of_philosophers)
		{
			pthread_mutex_destroy(&data->fork[j]);
			j++;
		}
		pthread_mutex_destroy(&data->print_lock);
		pthread_mutex_destroy(&data->death);
		pthread_mutex_destroy(&data->dined);
		pthread_mutex_destroy(&data->meals_eaten_mutex);
		pthread_mutex_destroy(&data->last_meal_timestamps_mutex);
		pthread_mutex_destroy(&data->dining_mutex);
		free(data->fork);
		return (1);
	}
	return (0);
}

int	initialize_mutexes(t_philosopher *philosophers, t_data *data)
{
	int	i;

	i = 0;
	(void)philosophers;
	data->fork = (pthread_mutex_t *)ft_calloc(data->number_of_philosophers,
			sizeof(pthread_mutex_t));
	if (data->fork == NULL)
	{
		write(2, "Error: calloc\n", 15);
		free(data);
		return (1);
	}
	while (i < data->number_of_philosophers)
	{
		initialize_mutex(&data->fork[i], data);
		i++;
	}
	initialize_mutex(&data->print_lock, data);
	initialize_mutex(&data->death, data);
	initialize_mutex(&data->dined, data);
	initialize_mutex(&data->meals_eaten_mutex, data);
	initialize_mutex(&data->last_meal_timestamps_mutex, data);
	initialize_mutex(&data->dining_mutex, data);
	return (0);
}

void	initialize_philosopher(t_philosopher *philosopher, int id,
		long start_time, t_data *data)
{
	philosopher->id = id;
	philosopher->last_meal_time = get_current_time();
	philosopher->meals_eaten = 0;
	philosopher->time_to_die = data->time_to_die;
	philosopher->data = data;
	philosopher->data->death_note = 0;
	philosopher->data->dined_enough = 0;
	pthread_mutex_lock(&data->last_meal_timestamps_mutex);
	data->last_meal_timestamps[id - 1] = start_time;
	pthread_mutex_unlock(&data->last_meal_timestamps_mutex);
}

void	initialize_multiple_philosophers(t_philosopher *philosophers,
		t_data *data)
{
	int		i;
	long	start_time;

	start_time = get_current_time();
	i = 0;
	while (i < data->number_of_philosophers)
	{
		data->start_time = start_time;
		initialize_philosopher(&philosophers[i], i + 1, start_time, data);
		assign_forks(&philosophers[i], data);
		i++;
	}
}

int	initialize_philosophers(t_data *data)
{
	t_philosopher	*philosophers;

	philosophers = allocate_philosophers(data->number_of_philosophers);
	data->philosophers = philosophers;
	if (initialize_mutexes(philosophers, data) != 0)
	{
		write(2, "Error: initialize_mutexes\n", 25);
		return (1);
	}
	initialize_multiple_philosophers(philosophers, data);
	return (0);
}
