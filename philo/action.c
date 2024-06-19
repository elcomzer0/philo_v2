/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 18:21:10 by jorgonca          #+#    #+#             */
/*   Updated: 2024/06/19 12:41:21 by jorgonca         ###   ########.fr       */
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
	//pthread_mutex_lock(&philo->data->dining_mutex);
	pthread_mutex_lock(&philo->data->death);
	if (philo->data->death_note)
	{
		pthread_mutex_unlock(&philo->data->death);
		//pthread_mutex_unlock(&philo->data->dining_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->data->death);
	pthread_mutex_lock(philo->left_fork);
	pthread_mutex_lock(philo->right_fork);
	print_status(philo, "has taken a fork");
	print_status(philo, "has taken a fork");
	//pthread_mutex_unlock(&philo->data->dining_mutex);
}
