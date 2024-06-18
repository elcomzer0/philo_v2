/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 10:05:43 by jorgonca          #+#    #+#             */
/*   Updated: 2024/06/18 19:33:40 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */




#include "philo.h"

int dining_checker(t_philosopher *philo)
{
    pthread_mutex_lock(&philo->data->meals_eaten_mutex);
    int result = (philo->data->times_must_eat != -1 && philo->meals_eaten >= philo->data->times_must_eat);
    if (result)
    {
        pthread_mutex_unlock(&philo->data->meals_eaten_mutex);
        return 1;
    }
    else
    {
        pthread_mutex_unlock(&philo->data->meals_eaten_mutex);   
        return 0;
    }
    //return result;
}

/**
 * Checks if the philosopher has a "death note" set.
 *
 * @param philo The philosopher to check.
 * @return 1 if the philosopher has a "death note" set, 0 otherwise.
 */

int death_note_check(t_philosopher *philo)
{
    if (philo == NULL || philo->data == NULL)
        return (0);
    pthread_mutex_lock(&philo->data->death);
    int death_note = philo->data->death_note; // Copy the value of the death note
    pthread_mutex_unlock(&philo->data->death);

    return death_note;
}


int dining_timespan(t_philosopher *philo)
{
    long current_time = get_current_time();
    return (current_time - philo->last_meal_time) >= philo->data->time_to_die;
}

void process_philo_life_cycle(t_data *data)
{
    //int unlocked_inside_loop = 0; // Flag to track if the mutex was unlocked inside the loop
    int i;

    pthread_mutex_lock(&data->last_meal_timestamps_mutex);
    long long current_time = get_current_time();
    i = 0;

    while (i < data->number_of_philosophers)
    {
        t_philosopher *philo = &data->philosophers[i];
        long long time_since_last_meal = current_time - data->last_meal_timestamps[i];
       // if (time_since_last_meal >= data->time_to_die - 50)
         //   printf("Time since last meal %lld time to die %ld\n", time_since_last_meal, data->time_to_die);
        if (time_since_last_meal >= data->time_to_die && data->death_note == 0) //TODO: verifica el caso de mandar a dormir mucho tiempo
        {
            //if (dining_timespan(philo)) // Use dining_timespan to check if the philosopher should die
            //{
                    //printf("\nTIME TO DIE\n");
                    pthread_mutex_lock(&data->death);
                   // if (!data->death_note) // Ensure death is reported only once
                   // {
                        data->death_note = 1;
                        pthread_mutex_unlock(&data->death);
                        print_status(philo, "died");
                        
                        //unlocked_inside_loop = 1;
                        break ;
                  //  }
                    //pthread_mutex_unlock(&data->death);
            //}
        }
        i++;
    }
    // if (!unlocked_inside_loop) // Only unlock if it wasn't already unlocked inside the loop
    // {
    //     pthread_mutex_unlock(&data->last_meal_timestamps_mutex);
    // }
    pthread_mutex_unlock(&data->last_meal_timestamps_mutex);
}

//
bool stop_simulation(t_data *data)
{
    bool stop = false;

    pthread_mutex_lock(&data->death);
    if (data->death_note == 1)
    {
        stop = true;
    }
    pthread_mutex_unlock(&data->death);

    if (!stop)
    {
        pthread_mutex_lock(&data->dined);
        if (data->dined_enough == 1)
        {
            stop = true;
        }
        pthread_mutex_unlock(&data->dined);
    }
    return stop;
}


void   *monitor_death(void *arg)
{
    t_data *data = (t_data *)arg;
    while(!stop_simulation(data))
    {
        //pthread_mutex_lock(&data->death);
        //if (!data->dined_enough)
            process_philo_life_cycle(data);
        //pthread_mutex_unlock(&data->death);
        usleep(100); // Short delay to reduce CPU usage
    }
    return NULL;
}

int check_eaten_status(t_data *data) //, int *total_dining)
{
    int count = 0;
    int i = 0;
    while (i < data->number_of_philosophers)
    {
        t_philosopher *philo = &data->philosophers[i];
        pthread_mutex_lock(&data->dined); //TODO: CHECK THIS THREAD CASE
        if (dining_checker(philo) == 1)
            count++;
        if (count == data->number_of_philosophers)
        {
            pthread_mutex_unlock(&data->dined);
            data->dined_enough = 1;
            return 1;
        }
        pthread_mutex_unlock(&data->dined);
        i++;
    }
    return (0);
}

void *monitor_eat(void *arg)
{
    t_data *data = (t_data *)arg;
    while(!stop_simulation(data)) //&& dined_enough(data->philosophers) == 0)
    {
        if (check_eaten_status(data) == 1)
            return (NULL);
        usleep(100); // Short delay to reduce CPU usage
    }
    return (NULL);
}




