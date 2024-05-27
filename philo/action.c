/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 18:21:10 by jorgonca          #+#    #+#             */
/*   Updated: 2024/05/27 17:53:12 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void acquire_forks(t_philosopher *philo, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
{
    while (1)
    {
        pthread_mutex_lock(&philo->data->fork_status_mutex);
        if (*(philo->l_fork) == 0 && *(philo->r_fork) == 0) {
            pthread_mutex_lock(first_fork);
            pthread_mutex_lock(second_fork);
            *(philo->l_fork) = 1;
            *(philo->r_fork) = 1;
            pthread_mutex_unlock(&philo->data->fork_status_mutex);
            print_status(philo, "has taken a fork");
            print_status(philo, "has taken a fork");
            philo->starvation_counter = 0; // Reset the starvation counter
            break;
        } else {
            pthread_mutex_unlock(&philo->data->fork_status_mutex);
            philo->starvation_counter++; // Increment the starvation counter
           // ft_usleep(400, philo); // Sleep for a short time to avoid busy waiting
            usleep(100); // Sleep for a short time to avoid busy waiting
        }
    }
}


void release_forks(t_philosopher *philo, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
{
    pthread_mutex_lock(&philo->data->fork_status_mutex);
    *(philo->l_fork) = 0;
    *(philo->r_fork) = 0;
    pthread_mutex_unlock(second_fork);
    pthread_mutex_unlock(first_fork);
    pthread_mutex_unlock(&philo->data->fork_status_mutex);
}



void update_last_meal_time(t_philosopher *philo)
{
    pthread_mutex_lock(&philo->data->last_meal_timestamps_mutex);
    philo->last_meal_time = get_current_time();
    philo->data->last_meal_timestamps[philo->id - 1] = philo->last_meal_time;
    pthread_mutex_unlock(&philo->data->last_meal_timestamps_mutex);
}


void action_eat(t_philosopher *philo)
{
    pthread_mutex_t *first_fork, *second_fork;
  
    if (philo->id % 2 == 0) {
        // Even philosophers acquire left fork first, then right fork
        first_fork = philo->right_fork;
        second_fork = philo->left_fork;
    } else {
        // Odd philosophers acquire right fork first, then left fork
        first_fork = philo->right_fork;
        second_fork = philo->left_fork;
    }
   

    acquire_forks(philo, first_fork, second_fork);

    update_last_meal_time(philo);
    print_status(philo, "is eating");
    ft_usleep(philo->data->time_to_eat, philo);

    release_forks(philo, first_fork, second_fork);
}

void action_sleep(t_philosopher *philo)
{
    print_status(philo, "is sleeping");
    ft_usleep(philo->data->time_to_sleep , philo);

}


void action_think(t_philosopher *philo)
{
    print_status(philo, "is thinking");

}
