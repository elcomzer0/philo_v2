/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 10:05:56 by jorgonca          #+#    #+#             */
/*   Updated: 2024/05/17 14:49:51 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */




#include "philo.h"

void *philosopher_routine(void *arg)
{
    t_philosopher *philo = (t_philosopher *)arg;
    t_data *data = philo->data;

    while (1)
    {
        // Thinking
        print_status(philo, "is thinking");

        // Pick up forks
        pthread_mutex_lock(&philo->left_fork);
        print_status(philo, "has taken a fork");
        pthread_mutex_lock(philo->right_fork);
        print_status(philo, "has taken a fork");

        // Eating
        print_status(philo, "is eating");
        philo->last_meal_time = get_current_time();
        philo->meals_eaten++;
        ft_usleep(data->time_to_eat * 1000);

        // Put down forks
        pthread_mutex_unlock(philo->right_fork);
        pthread_mutex_unlock(&philo->left_fork);

        // Sleeping
        print_status(philo, "is sleeping");
        ft_usleep(data->time_to_sleep * 1000);

        if (data->times_must_eat != -1 && philo->meals_eaten >= data->times_must_eat)
            break;
    }

    return NULL;
}

void start_simulation(t_data *data)
{
    pthread_mutex_init(&data->print_lock, NULL);
    
    int i = 0;
    while(i < data->number_of_philosophers)
    {
        data->philosophers[i].data = data;
        if (pthread_create(&data->philosophers[i].thread, NULL, philosopher_routine, &data->philosophers[i]))
        {
            printf("Error creating thread %d\n", i + 1);
            exit(EXIT_FAILURE);
        }
        i++;
    }

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
        pthread_mutex_destroy(&data->philosophers[i].left_fork);
        i++;
    }
}

