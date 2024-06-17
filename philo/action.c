/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 18:21:10 by jorgonca          #+#    #+#             */
/*   Updated: 2024/06/17 23:26:43 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdint.h>

/* 
    This function is used to update the last meal time of a philosopher.
    It is called when a philosopher finishes eating.
    It locks the last meal timestamps mutex, updates the philosopher's last meal time,
    and then unlocks the last meal timestamps mutex.
    
*/
void update_last_meal_time(t_philosopher *philo)
{
    pthread_mutex_lock(&philo->data->last_meal_timestamps_mutex);
    philo->last_meal_time = get_current_time(); // Update the last meal time of the philosopher
    philo->data->last_meal_timestamps[philo->id - 1] = philo->last_meal_time; // Update the last meal time in the array of last meal times. 

    pthread_mutex_unlock(&philo->data->last_meal_timestamps_mutex);
}



/*
    This function is used to release the forks of a philosopher.
    It locks the fork status mutex, sets the left and right fork variables to 0,
    and then unlocks the fork status mutex.
    It then unlocks the left and right forks.
*/
void release_forks(t_philosopher *philo)
{
    pthread_mutex_unlock(philo->left_fork);
    pthread_mutex_unlock(philo->right_fork);
}

void acquire_forks(t_philosopher *philo)
{
 
    pthread_mutex_lock(&philo->data->dining_mutex);
    pthread_mutex_lock(&philo->data->death); // Lock the death mutex to prevent the philosopher from eating if they are about to die
    if (philo->data->death_note) // if the philosopher has a death note, then release the forks and return
    {
        pthread_mutex_unlock(&philo->data->death);
        pthread_mutex_unlock(&philo->data->dining_mutex);
        return;
    }
    pthread_mutex_unlock(&philo->data->death);
    pthread_mutex_lock(philo->left_fork);
    pthread_mutex_lock(philo->right_fork);
    print_status(philo, "has taken a fork");
    print_status(philo, "has taken a fork");
    pthread_mutex_unlock(&philo->data->dining_mutex);
}


void action_eat(t_philosopher *philo)
{
    acquire_forks(philo);
    update_last_meal_time(philo);
    ft_usleep(philo->data->time_to_eat, philo);
    print_status(philo, "is eating");
    release_forks(philo);
}

void action_sleep(t_philosopher *philo)
{
    print_status(philo, "is sleeping");
    ft_usleep(philo->data->time_to_sleep, philo);
}

void action_think(t_philosopher *philo)
{
    print_status(philo, "is thinking");
}
