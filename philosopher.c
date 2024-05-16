/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 10:05:56 by jorgonca          #+#    #+#             */
/*   Updated: 2024/05/16 10:07:36 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */




#include "philo.h"

/**
 * The philosopher_routine function is the main entry point for each philosopher thread.
 * It implements the philosopher's behavior, including thinking, picking up forks, eating,
 * and sleeping. The function also checks if the monitoring thread has detected death or
 * completion of the simulation.
 *
 * @param arg A pointer to the t_philo struct containing the philosopher's data.
 * @return Always returns NULL.
 */
void *philosopher_routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    t_data *data = philo->data;

    while (1)
    {
        print_status(philo, "is thinking");

        // Picking up forks
        pthread_mutex_lock(&philo->left_fork);
        print_status(philo, "has taken a fork");
        pthread_mutex_lock(&philo->right_fork);
        print_status(philo, "has taken a fork");

        // Eating
        philo->last_meal = get_timestamp();
        print_status(philo, "is eating");
        usleep(data->time_to_eat * 1000);
        philo->meals_eaten++;

        // Putting down forks
        pthread_mutex_unlock(&philo->left_fork);
        pthread_mutex_unlock(&philo->right_fork);

        // Sleeping
        print_status(philo, "is sleeping");
        usleep(data->time_to_sleep * 1000);

        // Check if the monitoring thread has detected death or completion
        if (data->num_meals > 0 && philo->meals_eaten >= data->num_meals)
            break;
    }
    return NULL;
}