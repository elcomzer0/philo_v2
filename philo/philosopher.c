/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 10:05:56 by jorgonca          #+#    #+#             */
/*   Updated: 2024/05/26 13:53:05 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */




#include "philo.h"



/* void *philosopher_routine(void *arg)
{
    t_philosopher *philo = (t_philosopher *)arg;
    t_data *data = philo->data;
    
    //print_philosopher(philo);
    // print_data(data);
    printf("in philosopher_routine: Philosopher %d starting at time %ld\n", philo->id, get_current_time());

    while (!data->exiting && death_note_check(philo) == 0)
    {
        if (!data->exiting)
        {   
            if(philo->prioritize_eating == 1)
            {
                action_eat(philo);
                philo->prioritize_eating = 0;
                    
            }
            else 
            {
                //usleep(100); // Sleep for a short period to avoid busy waiting
                action_eat(philo);
                action_sleep(philo);
                action_think(philo);
                
            }
        }
    }
    if (!data->exiting)
    {    
        pthread_mutex_lock(&data->completed_threads_mutex);
        data->completed_threads_count++;
        pthread_mutex_unlock(&data->completed_threads_mutex);
    }
    return NULL;
} */

void *philosopher_routine(void *arg)
{
    t_philosopher *philo = (t_philosopher *)arg;
    t_data *data = philo->data;
    
    printf("in philosopher_routine: Philosopher %d starting at time %ld\n", philo->id, get_current_time());

    while (!data->exiting && death_note_check(philo) == 0)
    {
        if (!data->exiting)
        {   
            // Check if the philosopher's starvation counter exceeds the threshold
            if (philo->starvation_counter >= STARVATION_THRESHOLD)
            {
                // Set the prioritize_eating flag to indicate that the philosopher should prioritize eating
                philo->prioritize_eating = 1;
            }

            if (philo->prioritize_eating == 1)
            {
                // Prioritize eating if the flag is set
                action_eat(philo);
                philo->prioritize_eating = 0; // Reset the flag after eating
            }
            else 
            {
                // Regular routine: think, eat, sleep
                action_eat(philo);
                action_think(philo);
                action_sleep(philo);
            }
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
   // int i;
    
    pthread_mutex_lock(&data->completed_threads_mutex);
    while (data->completed_threads_count < data->number_of_philosophers)
    {
        pthread_mutex_unlock(&data->completed_threads_mutex);
        usleep(100); // Sleep for a short period to avoid busy waiting
        pthread_mutex_lock(&data->completed_threads_mutex);
    }
    pthread_mutex_unlock(&data->completed_threads_mutex);
/* 
    i = 0;
    while(i < data->number_of_philosophers)
    {
        pthread_join(data->philosophers[i].thread, NULL);
        i++;
    }
} */
}
