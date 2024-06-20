/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 10:05:56 by jorgonca          #+#    #+#             */
/*   Updated: 2024/06/20 14:31:43 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	meals_eaten(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->data->meals_eaten_mutex);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->data->meals_eaten_mutex);
}

int	ft_usleep_even(t_philosopher *philo)
{
	if (philo->id % 2 == 0)
	{
		if (ft_usleep(philo->data->time_to_eat / 2, philo) == 1)
			return (1);
	}
	return (0);
}

void	*philosopher_routine(void *arg)
{
	t_philosopher	*philo;
	t_data			*data;

	philo = (t_philosopher *)arg;
	data = philo->data;
	if (ft_usleep_even(philo) == 1)
		return (NULL);
	while (death_note_check(philo) == 0 && dined_enough_check(data) == 0)
	{
		if (data->number_of_philosophers < 2)
		{
			if (print_status(philo, "has taken a fork") == 1)
				return (NULL);
			ft_usleep(philo->time_to_die, philo);
			return (NULL);
		}
		if (action_eat(philo) == 1)
			return (NULL);
		meals_eaten(philo);
		if (action_sleep(philo) == 1)
			return (NULL);
		if (action_think(philo) == 1)
			return (NULL);
	}
	return (NULL);
}
