/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 10:05:56 by jorgonca          #+#    #+#             */
/*   Updated: 2024/06/18 23:53:04 by jorgonca         ###   ########.fr       */
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
		ft_usleep(philo->data->time_to_eat - 10, philo);
	while (death_note_check(philo) == 0 && dined_enough_check(data) == 0)
	{
		action_eat(philo);
		pthread_mutex_lock(&data->meals_eaten_mutex);
		philo->meals_eaten++;
		pthread_mutex_unlock(&data->meals_eaten_mutex);
		action_sleep(philo);
		action_think(philo);
	}
	return (NULL);
}
