/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 18:21:10 by jorgonca          #+#    #+#             */
/*   Updated: 2024/05/25 01:48:45 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"



/* void action_eat(t_philosopher *philo)
{
    pthread_mutex_t *first_fork, *second_fork;

    if (philo->id % 2 == 0)
    {
        // Even philosophers acquire left fork first, then right fork
        first_fork = philo->left_fork;
        second_fork = philo->right_fork;
    }
    else
    {
        // Odd philosophers acquire right fork first, then left fork
        first_fork = philo->right_fork;
        second_fork = philo->left_fork;
    }

    if (pthread_mutex_lock(first_fork) != 0)
    {
        // Handle error
        return;
    }
    print_status(philo, "has taken a fork");

    if (pthread_mutex_lock(second_fork) != 0)
    {
        // Handle error and unlock first_fork
        pthread_mutex_unlock(first_fork);
        return;
    }
    print_status(philo, "has taken a fork");
    
     // Update the last_meal timestamp
    pthread_mutex_lock(&philo->data->last_meal_timestamps_mutex);
    philo->last_meal_time = get_current_time();
    philo->data->last_meal_timestamps[philo->id - 1] = philo->last_meal_time;
    pthread_mutex_unlock(&philo->data->last_meal_timestamps_mutex);

    print_status(philo, "is eating");
    ft_usleep(philo->data->time_to_eat * 1000, philo);

    // Update the last_meal timestamp
    philo->last_meal_time = get_current_time();

    // Acquire the meals_eaten_mutex
    pthread_mutex_lock(&philo->data->meals_eaten_mutex);
    philo->meals_eaten++;
    pthread_mutex_unlock(&philo->data->meals_eaten_mutex);

    // Check if l_fork and r_fork are valid before dereferencing
    if (philo->l_fork != NULL && philo->r_fork != NULL)
    {
        *(philo->l_fork) = 0;
        *(philo->r_fork) = 0;
    }

    pthread_mutex_unlock(second_fork);
    pthread_mutex_unlock(first_fork);
} */

void action_eat(t_philosopher *philo)
{
    pthread_mutex_t *first_fork, *second_fork;

    // Determine the order of fork acquisition to prevent deadlocks
    if (philo->left_fork < philo->right_fork) {
        first_fork = philo->left_fork;
        second_fork = philo->right_fork;
    } else {
        first_fork = philo->right_fork;
        second_fork = philo->left_fork;
    }

    if (pthread_mutex_lock(first_fork) != 0) {
        // Handle error
        return;
    }
    print_status(philo, "has taken a fork");

    if (pthread_mutex_lock(second_fork) != 0) {
        // Handle error and unlock first_fork
        pthread_mutex_unlock(first_fork);
        return;
    }
    print_status(philo, "has taken a fork");

    // Update the last_meal timestamp
    pthread_mutex_lock(&philo->data->last_meal_timestamps_mutex);
    philo->last_meal_time = get_current_time();
    philo->data->last_meal_timestamps[philo->id - 1] = philo->last_meal_time;
    pthread_mutex_unlock(&philo->data->last_meal_timestamps_mutex);

    print_status(philo, "is eating");
    ft_usleep(philo->data->time_to_eat * 1000, philo);

    // Update the last_meal timestamp
    philo->last_meal_time = get_current_time();

    // Acquire the meals_eaten_mutex
    pthread_mutex_lock(&philo->data->meals_eaten_mutex);
    philo->meals_eaten++;
    pthread_mutex_unlock(&philo->data->meals_eaten_mutex);

    // Check if l_fork and r_fork are valid before dereferencing
    if (philo->l_fork != NULL && philo->r_fork != NULL) {
        *(philo->l_fork) = 0;
        *(philo->r_fork) = 0;
    }

    pthread_mutex_unlock(second_fork);
    pthread_mutex_unlock(first_fork);
}



void action_sleep(t_philosopher *philo)
{
    print_status(philo, "is sleeping");
    ft_usleep(philo->data->time_to_sleep * 1000, philo);

}


void action_think(t_philosopher *philo)
{
    print_status(philo, "is thinking");

}
