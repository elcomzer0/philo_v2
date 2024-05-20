/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 10:05:56 by jorgonca          #+#    #+#             */
/*   Updated: 2024/05/20 21:24:54 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */




#include "philo.h"

void *philosopher_routine(void *arg)
{
    t_philosopher *philo = (t_philosopher *)arg;
    //t_data *data = philo->data;

    while (death_note_check(philo) == 0)
    {
        action_eat(philo);
        action_sleep(philo);
        action_think(philo);
    }

    return NULL;
}

void start_simulation(t_data *data)
{
    int i;
    //pthread_mutex_init(&data->print_lock, NULL);
    //printf("Starting simulation\n" );
    

    i = 0;
    while(i < data->number_of_philosophers)
    {
        pthread_join(data->philosophers[i].thread, NULL);
        i++;
    }

    pthread_mutex_destroy(&data->print_lock);
    
    i = 0;
    while(i < data->number_of_philosophers)
    {
        pthread_mutex_destroy(data->philosophers[i].left_fork);
        i++;
    }
}

