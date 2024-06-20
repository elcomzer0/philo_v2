/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 21:19:44 by jorgonca          #+#    #+#             */
/*   Updated: 2024/06/20 18:44:19 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	stop_simulation(t_data *data)
{
	bool	stop;

	stop = false;
	pthread_mutex_lock(&data->death);
	if (data->death_note == 1 || data->all_philosophers_created == 1)
	{
		stop = true;
	}
	pthread_mutex_unlock(&data->death);
	if (!stop)
	{
		if (dined_enough_check(data) == 1)
		{
			stop = true;
		}
	}
	return (stop);
}

int	check_eaten_status(t_data *data)
{
	int				count;
	int				i;
	t_philosopher	*philo;

	count = 0;
	i = 0;
	while (i < data->number_of_philosophers)
	{
		philo = &data->philosophers[i];
		if (dining_checker(philo) == 1)
			count++;
		if (count == data->number_of_philosophers)
		{
			pthread_mutex_lock(&data->dined);
			data->dined_enough = 1;
			pthread_mutex_unlock(&data->dined);
			return (1);
		}
		i++;
	}
	return (0);
}

int	death_note_check(t_philosopher *philo)
{
	int	death_note;

	if (philo == NULL || philo->data == NULL)
		return (0);
	pthread_mutex_lock(&philo->data->death);
	death_note = philo->data->death_note;
	pthread_mutex_unlock(&philo->data->death);
	return (death_note);
}

int	dined_enough_check(t_data *data)
{
	int	dined_enough;

	if (data == NULL)
		return (0);
	pthread_mutex_lock(&data->dined);
	dined_enough = data->dined_enough;
	pthread_mutex_unlock(&data->dined);
	return (dined_enough);
}
