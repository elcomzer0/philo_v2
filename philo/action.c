/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 18:21:10 by jorgonca          #+#    #+#             */
/*   Updated: 2024/06/18 21:12:04 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdint.h>

void	update_last_meal_time(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->data->last_meal_timestamps_mutex);
	philo->last_meal_time = get_current_time();
	philo->data->last_meal_timestamps[philo->id - 1] = philo->last_meal_time;
	pthread_mutex_unlock(&philo->data->last_meal_timestamps_mutex);
}

void	release_forks(t_philosopher *philo)
{
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
}

void	acquire_forks(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->data->dining_mutex);
	pthread_mutex_lock(&philo->data->death);
	if (philo->data->death_note)
	{
		pthread_mutex_unlock(&philo->data->death);
		pthread_mutex_unlock(&philo->data->dining_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->data->death);
	pthread_mutex_lock(philo->left_fork);
	pthread_mutex_lock(philo->right_fork);
	print_status(philo, "has taken a fork");
	print_status(philo, "has taken a fork");
	pthread_mutex_unlock(&philo->data->dining_mutex);
}

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
	print_status(philo, "is sleeping");
	ft_usleep(philo->data->time_to_sleep, philo);
}

void	action_think(t_philosopher *philo)
{
	if (philo->data->time_to_eat - philo->data->time_to_sleep >= 0
		&& philo->data->number_of_philosophers % 2 == 1)
		ft_usleep((philo->data->time_to_eat - philo->data->time_to_sleep) + 2,
			philo);
	print_status(philo, "is thinking");
}
