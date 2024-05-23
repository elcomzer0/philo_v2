/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 18:21:10 by jorgonca          #+#    #+#             */
/*   Updated: 2024/05/23 14:48:57 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* void action_eat(t_philosopher *philo)
{
    if (philo->id % 2 == 0)
        usleep(50);
    pthread_mutex_lock(philo->left_fork);
    *(philo->l_fork) = 1;
    print_status(philo, "has taken a fork");
    pthread_mutex_lock(philo->right_fork);
    *(philo->r_fork) = 1;
    print_status(philo, "has taken a fork");

    print_status(philo, "is eating");
    ft_usleep(philo->data->time_to_eat * 1000, philo);
    
    *(philo->l_fork) = 0;
    *(philo->r_fork) = 0;
    pthread_mutex_unlock(philo->left_fork);
    pthread_mutex_unlock(philo->right_fork);



    //TODO für das BEheben der lock oder violation müssen diw geraden zuerst das linke dann das rechte fork nehmen und die ungerade genau anders herum
    // todo time last ate updaten

} */

void action_eat(t_philosopher *philo)
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

    pthread_mutex_lock(first_fork);
    print_status(philo, "has taken a fork");
    pthread_mutex_lock(second_fork);
    print_status(philo, "has taken a fork");

    print_status(philo, "is eating");
    ft_usleep(philo->data->time_to_eat * 1000, philo);

    // Update the last_meal timestamp
    philo->last_meal_time = get_current_time();

    *(philo->l_fork) = 0;
    *(philo->r_fork) = 0;
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
