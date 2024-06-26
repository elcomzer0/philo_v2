/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 10:05:43 by jorgonca          #+#    #+#             */
/*   Updated: 2024/06/20 15:16:07 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	dining_checker(t_philosopher *philo)
{
	int	result;

	pthread_mutex_lock(&philo->data->meals_eaten_mutex);
	result = (philo->data->times_must_eat != -1
			&& philo->meals_eaten >= philo->data->times_must_eat);
	if (result)
	{
		pthread_mutex_unlock(&philo->data->meals_eaten_mutex);
		return (1);
	}
	else
	{
		pthread_mutex_unlock(&philo->data->meals_eaten_mutex);
		return (0);
	}
}

void	death_is_one(t_data *data)
{
	pthread_mutex_lock(&data->death);
	data->death_note = 1;
	pthread_mutex_unlock(&data->death);
}

int	process_philo_life_cycle(t_data *data)
{
	int				i;
	long long		current_time;
	long long		time_since_last_meal;
	t_philosopher	*philo;

	pthread_mutex_lock(&data->last_meal_timestamps_mutex);
	current_time = get_current_time();
	if (current_time == -1)
		return (1);
	i = 0;
	while (i < data->number_of_philosophers)
	{
		philo = &data->philosophers[i];
		time_since_last_meal = current_time - data->last_meal_timestamps[i];
		if (time_since_last_meal >= data->time_to_die && data->death_note == 0)
		{
			death_is_one(data);
			print_status(philo, "died");
			break ;
		}
		i++;
	}
	pthread_mutex_unlock(&data->last_meal_timestamps_mutex);
	return (0);
}

void	*monitor_death(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (!stop_simulation(data))
	{
		process_philo_life_cycle(data);
		if (usleep(100) == -1)
		{
			write(2, "Error: usleep\n", 13);
			return (NULL);
		}
	}
	return (NULL);
}

void	*monitor_eat(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (!stop_simulation(data))
	{
		if (check_eaten_status(data) == 1)
			return (NULL);
		if (usleep(100) == -1)
		{
			write(2, "Error: usleep\n", 13);
			return (NULL);
		}
	}
	return (NULL);
}
