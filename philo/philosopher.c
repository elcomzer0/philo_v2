/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 10:05:56 by jorgonca          #+#    #+#             */
/*   Updated: 2024/05/24 17:53:02 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */




#include "philo.h"

/* void *philosopher_routine(void *arg)
{
    t_philosopher *philo = (t_philosopher *)arg;
    t_data *data = philo->data;

    while (death_note_check(philo) == 0)
    {
        action_eat(philo);
        action_sleep(philo);
        action_think(philo);
    }
    pthread_mutex_lock(&data->completed_threads_mutex);
    data->completed_threads_count++;
    pthread_mutex_unlock(&data->completed_threads_mutex);

    return NULL;
} */

void *philosopher_routine(void *arg)
{
    t_philosopher *philo = (t_philosopher *)arg;
    t_data *data = philo->data;
    
    /* print_philosopher(philo);
    print_data(data); */
    while (!data->exiting && death_note_check(philo) == 0)
    {
        if (!data->exiting)
        {   
            action_eat(philo);
            action_sleep(philo);
            action_think(philo);
        }
    }
    if (!data->exiting)
    {    
        pthread_mutex_lock(&data->completed_threads_mutex);
        data->completed_threads_count++;
        pthread_mutex_unlock(&data->completed_threads_mutex);
    }
    return NULL;
}

void start_simulation(t_data *data)
{
    int i;
    //pthread_mutex_init(&data->print_lock, NULL);
    //printf("Starting simulation\n" );
    
    pthread_mutex_lock(&data->completed_threads_mutex);
    while (data->completed_threads_count < data->number_of_philosophers)
    {
        pthread_mutex_unlock(&data->completed_threads_mutex);
        usleep(100); // Sleep for a short period to avoid busy waiting
        pthread_mutex_lock(&data->completed_threads_mutex);
    }
    pthread_mutex_unlock(&data->completed_threads_mutex);

    i = 0;
    while(i < data->number_of_philosophers)
    {
        pthread_join(data->philosophers[i].thread, NULL);
        i++;
    }

   // pthread_mutex_destroy(&data->print_lock);
    
    /* i = 0;
    while(i < data->number_of_philosophers)
    {
        pthread_mutex_destroy(data->philosophers[i].left_fork);
        i++;
    } */
}

