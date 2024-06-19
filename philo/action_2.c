/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 23:57:50 by jorgonca          #+#    #+#             */
/*   Updated: 2024/06/19 23:31:07 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdint.h>

int	action_eat(t_philosopher *philo)
{
	if (acquire_forks(philo) == 1)
		return 1;
	update_last_meal_time(philo);
	if (print_status(philo, "is eating") == 1)
		return 1;
	if (ft_usleep(philo->data->time_to_eat, philo) == 1)
		return 1;
	release_forks(philo);
	return 0;
}

int	action_sleep(t_philosopher *philo)
{
	if (dined_enough_check(philo->data) == 0) //|| death_note_check(philo) == 0)
	{
		if (print_status(philo, "is sleeping") == 1)
			return 1;
		if (ft_usleep(philo->data->time_to_sleep, philo) == 1)
			return 1;
	}
	return 0;
}

int	action_think(t_philosopher *philo)
{
	if (dined_enough_check(philo->data) == 0 ) //|| death_note_check(philo) == 0)
	{
		if (print_status(philo, "is thinking") == 1)
			return 1;
		if (philo->data->time_to_eat - philo->data->time_to_sleep >= 0 && philo->data->number_of_philosophers % 2 == 1)
		{
		 	if (ft_usleep((philo->data->time_to_eat - philo->data->time_to_sleep) + 1, philo) == 1)
				return 1;
		}
	}
	return 0;
}
