/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 10:05:43 by jorgonca          #+#    #+#             */
/*   Updated: 2024/05/20 15:23:54 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */




#include "philo.h"

int dining_checker(t_philosopher *philo)
{
    if (philo->data->times_must_eat != -1 && philo->meals_eaten >= philo->data->times_must_eat)
        return (1);
    return (0);
}
int dining_timespan(t_philosopher *philo)
{
   if ((get_current_time() - philo->last_meal_time) >= philo->data->time_to_die)
   {
        print_status(philo, "died");
        return (1);
   }
    return (0); 
}

/* int starving_check(t_philosopher *philo)
{
    int i;

    i = 0;
    while(dining_timespan(philo) == 0)
    {
        i++;
        if (i == philo->data->number_of_philosophers)
        {
            pthread_mutex_lock(&philo->data->death);
            philo->data->death_note = 1;
            pthread_mutex_unlock(&philo->data->death);
            return (1);
        }
    }
    return (0);
} */
 



int dined_enough(t_philosopher *philo)
{
    int i;
    int count;

    i = 0;
    count = 0;
    while(1 < philo->data->number_of_philosophers && dining_checker(philo) == 1)
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

int death_note_check(t_philosopher *philo)
{
    pthread_mutex_lock(&philo->data->death);
    if (philo->data->death_note == 1)
    {
        pthread_mutex_unlock(&philo->data->death);
        return (1);
    }
    pthread_mutex_unlock(&philo->data->death);
    return (0);
}


/*  void    monitor(t_data *data)
{
    
    while(1 && death_note_check(data->philosophers))
    {
        
        
    }  
} */

void process_philo_life_cycle(t_data *data, int *total_dining)
{
    for (int i = 0; i < data->number_of_philosophers; i++)
    {
        pthread_mutex_lock(&data->philosophers[i].data->death);
        if (dining_timespan(&data->philosophers[i]))
        {
            pthread_mutex_unlock(&data->philosophers[i].data->death);
            pthread_mutex_lock(&data->death);
            data->death_note = 1;
            pthread_mutex_unlock(&data->death);
            print_status(&data->philosophers[i], "died");
            break;
        }
        pthread_mutex_unlock(&data->philosophers[i].data->death);
        pthread_mutex_lock(&data->dined);
        if (dined_enough(&data->philosophers[i]))
        {
            (*total_dining)++;
        }
        pthread_mutex_unlock(&data->dined);
    }
}

void check_philo_death_and_eaten_status(t_data *data, int *total_dining)
{
    pthread_mutex_lock(&data->dined);
    if (*total_dining >= data->number_of_philosophers && data->times_must_eat > 0)
    {
        pthread_mutex_lock(&data->death);
        data->death_note = 1;
        pthread_mutex_unlock(&data->death);
    }
    pthread_mutex_unlock(&data->dined);
}

void *monitor_routine(void *arg)
{
    usleep(500);
    t_data *data = (t_data *)arg;
    while(!death_note_check(&data->philosophers[0]))
    {
        int eaten = 0;
        process_philo_life_cycle(data, &eaten);
        check_philo_death_and_eaten_status(data, &eaten);
    }
    return NULL;
}