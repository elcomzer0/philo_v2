/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 10:05:56 by jorgonca          #+#    #+#             */
/*   Updated: 2024/06/19 23:30:22 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philosopher_routine(void *arg)
{
	t_philosopher	*philo;
	t_data			*data;

	philo = (t_philosopher *)arg;
	data = philo->data;
	if (philo->id % 2 == 0)
	{
		if (ft_usleep(philo->data->time_to_eat / 2, philo) == 1)
			return (NULL);
	}
	while (death_note_check(philo) == 0 && dined_enough_check(data) == 0)
	{
		if (data->number_of_philosophers < 2)
		{
			if (print_status(philo, "has taken a fork") == 1)
				return (NULL);
			ft_usleep(philo->time_to_die, philo);
			return (NULL);
		}
		if(action_eat(philo) == 1)
			return (NULL);
		pthread_mutex_lock(&data->meals_eaten_mutex);
		philo->meals_eaten++;
		pthread_mutex_unlock(&data->meals_eaten_mutex);
		if(action_sleep(philo) == 1)
			return (NULL);
		if(action_think(philo) == 1)
			return (NULL);
	}	
	return (NULL);
}
