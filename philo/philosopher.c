/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 10:05:56 by jorgonca          #+#    #+#             */
/*   Updated: 2024/06/08 13:43:25 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */




#include "philo.h"

void *philosopher_routine(void *arg)
{
    t_philosopher *philo = (t_philosopher *)arg;
    t_data *data = philo->data;
    
  //  printf("in philosopher_routine: Philosopher %d starting at time %ld\n", philo->id, get_current_time());

    //printf("in philosopher_routine times must eat: %d\n", data->times_must_eat);
    
    /* explantion: data->exiting is a flag that is set to 1 when the program is shutting down.
        if the flag is set to 1, then the philosopher routine should exit.
        death_note_check() checks if the philosopher's death note has been set to 1.
        If the death note has been set to 1, then the philosopher routine should exit.
        data->times_must_eat is a flag that is set to -1 when the program is running.
        If the flag is set to -1, then the philosopher routine should run forever.
        If the flag is set to a positive number, then the philosopher routine should run for that many times.
        If the flag is set to 0, then the philosopher routine should run forever.
        philo->meals_eaten is a counter that is incremented every time the philosopher eats.
    */
    while (!data->exiting && death_note_check(philo) == 0 && (data->times_must_eat == -1 
        || philo->meals_eaten < data->times_must_eat))
    {
        if (!data->exiting)
        {   
                action_eat(philo);
                //usleep(100);
                pthread_mutex_lock(&data->meals_eaten_mutex);
                philo->meals_eaten++;
                pthread_mutex_unlock(&data->meals_eaten_mutex);
                action_sleep(philo);
                action_think(philo);
            
        }
    }
    //printf("philosopher %d sleeping lengthe: %ld\n", philo->id, get_current_time() - philo->last_meal_time);
    //printf("first philosopher sleeping lengthe: %ld\n", get_current_time() - philo->last_meal_time);
    
    if (!data->exiting) // If the philosopher is still alive, increment the completed threads counter
    {    
        pthread_mutex_lock(&data->completed_threads_mutex);
        data->completed_threads_count++;
        pthread_mutex_unlock(&data->completed_threads_mutex);
    }
    
    return NULL;
}


// has to be renamed
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
