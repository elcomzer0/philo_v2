/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 18:21:10 by jorgonca          #+#    #+#             */
/*   Updated: 2024/05/20 21:36:26 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void action_eat(t_philosopher *philo)
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
