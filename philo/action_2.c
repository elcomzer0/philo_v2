/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 23:57:50 by jorgonca          #+#    #+#             */
/*   Updated: 2024/06/19 21:25:43 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdint.h>

void	action_eat(t_philosopher *philo)
{
	acquire_forks(philo);
	update_last_meal_time(philo);
	print_status(philo, "is eating");
	ft_usleep(philo->data->time_to_eat, philo);
	release_forks(philo);
}

void	action_sleep(t_philosopher *philo)
{
	if (dined_enough_check(philo->data) == 0) //|| death_note_check(philo) == 0)
	{
		print_status(philo, "is sleeping");
		ft_usleep(philo->data->time_to_sleep, philo);
	}
}

void	action_think(t_philosopher *philo)
{
	if (dined_enough_check(philo->data) == 0 ) //|| death_note_check(philo) == 0)
	{
		print_status(philo, "is thinking");
		if (philo->data->time_to_eat - philo->data->time_to_sleep >= 0 && philo->data->number_of_philosophers % 2 == 1)
		 	ft_usleep((philo->data->time_to_eat - philo->data->time_to_sleep) + 1, philo);
	}
}
