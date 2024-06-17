/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 10:05:43 by jorgonca          #+#    #+#             */
/*   Updated: 2024/06/17 23:30:29 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */




#include "philo.h"

int dining_checker(t_philosopher *philo)
{
    pthread_mutex_lock(&philo->data->meals_eaten_mutex);
    int result = (philo->data->times_must_eat != -1 && philo->meals_eaten >= philo->data->times_must_eat);
    pthread_mutex_unlock(&philo->data->meals_eaten_mutex);
    return result;
}


/**
 * Checks if a philosopher has dined enough times.
 *
 * This function checks if a philosopher has dined the required number of times, 
 * which is determined by the `number_of_philosophers` field in the `t_philosopher` struct.
 *  It does this by iterating through all the philosophers and checking if the `dining_checker`
 *  function returns 1 for each one. If the count reaches the `number_of_philosophers`,
 *  it sets the `dined_enough` field in the `t_philosopher` struct to 1 and returns 1 
 * to indicate that the philosopher has dined enough.
 *
 * @param philo The philosopher to check.
 * @return 1 if the philosopher has dined enough, 0 otherwise.
 */
int dined_enough(t_philosopher *philo)
{
    int i;
    int count;

    i = 0;
    count = 0;
    while (1 < philo->data->number_of_philosophers && dining_checker(philo) == 1)
    {
        count++;
        if (count == philo->data->number_of_philosophers)
        {
            pthread_mutex_lock(&philo->data->dined);
            philo->data->dined_enough = 1;
            pthread_mutex_unlock(&philo->data->dined);
            return (1);
        }
        i++;
    }
    return (0);
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
    long long current_time = get_current_time();
    int unlocked_inside_loop = 0; // Flag to track if the mutex was unlocked inside the loop
    int i;

    pthread_mutex_lock(&data->last_meal_timestamps_mutex);
    i = 0;

    while (i < data->number_of_philosophers)
    {
        t_philosopher *philo = &data->philosophers[i];
        long long time_since_last_meal = current_time - data->last_meal_timestamps[i];
        if (time_since_last_meal >= data->time_to_die)
        {
            if (dining_timespan(philo)) // Use dining_timespan to check if the philosopher should die
            {
               
                    pthread_mutex_lock(&data->death);
                    if (!data->death_note) // Ensure death is reported only once
                    {
                        data->death_note = 1;
                        pthread_mutex_unlock(&data->death);
                        print_status(philo, "died");
                        pthread_mutex_unlock(&data->last_meal_timestamps_mutex);
                        unlocked_inside_loop = 1;
                        break;
                    }
                    pthread_mutex_unlock(&data->death);
            }
        }
        i++;
    }
    if (!unlocked_inside_loop) // Only unlock if it wasn't already unlocked inside the loop
    {
        pthread_mutex_unlock(&data->last_meal_timestamps_mutex);
    }
}





//
bool stop_simulation(t_data *data)
{
    bool stop = false;

    pthread_mutex_lock(&data->death);
    if (data->death_note)
    {
        stop = true;
    }
    pthread_mutex_unlock(&data->death);

    if (!stop)
    {
        pthread_mutex_lock(&data->dined);
        if (data->dined_enough)
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
        process_philo_life_cycle(data); //, &eaten);
        
        usleep(1000); // Short delay to reduce CPU usage
    }
    return NULL;
}

void check_eaten_status(t_data *data) //, int *total_dining)
{
    pthread_mutex_lock(&data->dined);
    if (data->times_must_eat > 0 && data->philosophers->meals_eaten >= data->number_of_philosophers)
    {
        data->dined_enough = 1;
        pthread_mutex_unlock(&data->dined);
    }
    else
    {
        pthread_mutex_unlock(&data->dined);
    }
}

void *monitor_eat(void *arg)
{
    t_data *data = (t_data *)arg;
    while(!stop_simulation(data))
    {
        check_eaten_status(data);
        usleep(1000); // Short delay to reduce CPU usage
    }
    return (NULL);
}

