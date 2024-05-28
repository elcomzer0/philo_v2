/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 10:05:56 by jorgonca          #+#    #+#             */
/*   Updated: 2024/05/28 01:36:07 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */




#include "philo.h"

void *philosopher_routine(void *arg)
{
    t_philosopher *philo = (t_philosopher *)arg;
    t_data *data = philo->data;
    
  //  printf("in philosopher_routine: Philosopher %d starting at time %ld\n", philo->id, get_current_time());

    //printf("in philosopher_routine times must eat: %d\n", data->times_must_eat);
    while (!data->exiting && death_note_check(philo) == 0 && (data->times_must_eat == -1 
        || philo->meals_eaten < data->times_must_eat))
    {
        if (!data->exiting)
        {   
            // Check if the philosopher's starvation counter exceeds the threshold
            
                // Regular routine: think, eat, sleep
                //usleep(1000); // Sleep for a short period to avoid busy waiting
                action_eat(philo);
                pthread_mutex_lock(&data->meals_eaten_mutex);
                philo->meals_eaten++;
                pthread_mutex_unlock(&data->meals_eaten_mutex);
                action_think(philo);
                action_sleep(philo);
            
        }
    }
    //sleeping length of philosopher id number 1
    //printf("philosopher %d sleeping lengthe: %ld\n", philo->id, get_current_time() - philo->last_meal_time);
    //printf("first philosopher sleeping lengthe: %ld\n", get_current_time() - philo->last_meal_time);
    
    if (!data->exiting)
    {    
        pthread_mutex_lock(&data->completed_threads_mutex);
        data->completed_threads_count++;
        pthread_mutex_unlock(&data->completed_threads_mutex);
    }
    
    return NULL;
}

/* void *philosopher_routine(void *arg)
{
    t_philosopher *philo = (t_philosopher *)arg;
    t_data *data = philo->data;
    
  //  printf("in philosopher_routine: Philosopher %d starting at time %ld\n", philo->id, get_current_time());

    //printf("in philosopher_routine times must eat: %d\n", data->times_must_eat);
    while (!data->exiting && death_note_check(philo) == 0 && (data->times_must_eat == -1 
        || philo->meals_eaten < data->times_must_eat))
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
                pthread_mutex_lock(&data->meals_eaten_mutex);
                philo->meals_eaten++;
                pthread_mutex_unlock(&data->meals_eaten_mutex);
                philo->prioritize_eating = 0; // Reset the flag after eating
            }
            else 
            {
                // Regular routine: think, eat, sleep
                //usleep(1000); // Sleep for a short period to avoid busy waiting
                action_eat(philo);
                pthread_mutex_lock(&data->meals_eaten_mutex);
                philo->meals_eaten++;
                pthread_mutex_unlock(&data->meals_eaten_mutex);
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
} */

/* void *philosopher_routine(void *arg)
{
    t_philosopher *philo = (t_philosopher *)arg;
    t_data *data = philo->data;

    while (!data->exiting && death_note_check(philo) == 0 && (data->times_must_eat == -1
        || philo->meals_eaten < data->times_must_eat))
    {
        if (!data->exiting)
        {
            // Wait for the philosopher's turn
            while (data->turn != philo->id)
            {
                // Busy wait or use a condition variable to wait for the turn
                usleep(10); // Sleep for a short period to avoid busy waiting
            }

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
                pthread_mutex_lock(&data->meals_eaten_mutex);
                philo->meals_eaten++;
                pthread_mutex_unlock(&data->meals_eaten_mutex);
                philo->prioritize_eating = 0; // Reset the flag after eating
            }
            else
            {
                // Regular routine: think, eat, sleep
                action_eat(philo);
                pthread_mutex_lock(&data->meals_eaten_mutex);
                philo->meals_eaten++;
                pthread_mutex_unlock(&data->meals_eaten_mutex);
                action_think(philo);
                action_sleep(philo);
            }

            // Pass the turn to the next philosopher
            data->turn = (data->turn + 1) % data->number_of_philosophers;
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



void start_simulation(t_data *data)
{
   // int i;
    
    pthread_mutex_lock(&data->completed_threads_mutex);
    while (data->completed_threads_count < data->number_of_philosophers)
    {
        pthread_mutex_unlock(&data->completed_threads_mutex);
        usleep(50); // Sleep for a short period to avoid busy waiting
        pthread_mutex_lock(&data->completed_threads_mutex);
    }
    pthread_mutex_unlock(&data->completed_threads_mutex);

}
