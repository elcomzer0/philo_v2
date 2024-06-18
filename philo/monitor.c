/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 10:05:43 by jorgonca          #+#    #+#             */
/*   Updated: 2024/06/18 20:08:06 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */




#include "philo.h"

int dining_checker(t_philosopher *philo)
{
    int result;
    
    pthread_mutex_lock(&philo->data->meals_eaten_mutex);
    result = (philo->data->times_must_eat != -1 && philo->meals_eaten >= philo->data->times_must_eat);
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
}

int death_note_check(t_philosopher *philo)
{
    int death_note;
    
    if (philo == NULL || philo->data == NULL)
        return (0);
    pthread_mutex_lock(&philo->data->death);
    death_note = philo->data->death_note;
    pthread_mutex_unlock(&philo->data->death);
    return death_note;
}

int dining_timespan(t_philosopher *philo)
{
    long current_time;
    
    current_time = get_current_time();
    return (current_time - philo->last_meal_time) >= philo->data->time_to_die;
}

void process_philo_life_cycle(t_data *data)
{  
    int i;
    long long current_time;
    long long time_since_last_meal;

    pthread_mutex_lock(&data->last_meal_timestamps_mutex);
    current_time = get_current_time();
    i = 0;

    while (i < data->number_of_philosophers)
    {
        t_philosopher *philo = &data->philosophers[i];
        time_since_last_meal = current_time - data->last_meal_timestamps[i];
       
        if (time_since_last_meal >= data->time_to_die && data->death_note == 0) //TODO: verifica el caso de mandar a dormir mucho tiempo
        {
            pthread_mutex_lock(&data->death);
            data->death_note = 1;
            pthread_mutex_unlock(&data->death);
            print_status(philo, "died");
            break ;
        }
        i++;
    }   
    pthread_mutex_unlock(&data->last_meal_timestamps_mutex);
}

bool stop_simulation(t_data *data)
{
    bool stop;
    
    stop = false;
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
    t_data *data;
    
    data = (t_data *)arg;
    while(!stop_simulation(data))
    {
        process_philo_life_cycle(data);
        usleep(100);
    }
    return NULL;
}

int check_eaten_status(t_data *data)
{
    int count = 0;
    int i = 0;
    t_philosopher *philo;
    
    while (i < data->number_of_philosophers)
    {
        philo = &data->philosophers[i];
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
    t_data *data;
    
    data = (t_data *)arg;
    while(!stop_simulation(data))
    {
        if (check_eaten_status(data) == 1)
            return (NULL);
        usleep(100);
    }
    return (NULL);
}




