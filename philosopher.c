/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 10:05:56 by jorgonca          #+#    #+#             */
/*   Updated: 2024/05/20 14:59:50 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */




#include "philo.h"

/* void *philosopher_routine(void *arg)
{
    t_philosopher *philo = (t_philosopher *)arg;
    t_data *data = philo->data;

    while (1)
    {
        // Thinking
        print_status(philo, "is thinking");

        ft_usleep(data->time_to_sleep * 1000);
        // Pick up forks
        pthread_mutex_lock(&philo->left_fork);
        print_status(philo, "has taken a fork");
        //lock the right fork
        if (philo->right_fork != NULL)
        {
            pthread_mutex_lock(philo->right_fork);
            print_status(philo, "has taken a fork");
        }
        // Eating
        print_status(philo, "is eating");
        ft_usleep(data->time_to_eat * 1000);
        philo->last_meal_time = get_current_time();
        philo->meals_eaten++;

        // Put down forks or unlock forks
        pthread_mutex_unlock(&philo->left_fork);
        if(philo->right_fork != NULL)
            pthread_mutex_unlock(philo->right_fork);

        // Sleeping
        print_status(philo, "is sleeping");
        ft_usleep(data->time_to_sleep * 1000);

        if (data->times_must_eat != -1 && philo->meals_eaten >= data->times_must_eat)
            break;
    }

    return NULL;
} */

void routine_eat(t_philosopher *philo)
{
    //eat
    print_status(philo, "is eating");
    ft_usleep(philo->data->time_to_eat * 1000);
    philo->last_meal_time = get_current_time();
    philo->meals_eaten++;
}

void routine_sleep(t_philosopher *philo)
{
    //sleep
    print_status(philo, "is sleeping");
    ft_usleep(philo->data->time_to_sleep * 1000);
}

void routine_think(t_philosopher *philo)
{
    //think
    print_status(philo, "is thinking");
}


/* void *routine_new(void *arg)
{
    //eat
    routine_eat((t_philosopher *)arg);

    
    //sleep
    routine_sleep((t_philosopher *)arg);
    
    //think
    routine_think((t_philosopher *)arg);
} */





void routine_loop(t_philosopher *philo)
{
    while (1 && )
    {
    routine_new(philo);
    }
}
void *philosopher_routine(void *arg)
{
    t_philosopher *philo = (t_philosopher *)arg;
    t_data *data = philo->data;

   while()

    return NULL;
}

void start_simulation(t_data *data)
{
    pthread_mutex_init(&data->print_lock, NULL);
    printf("Starting simulation\n" );
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
        pthread_mutex_destroy(data->philosophers[i].left_fork);
        i++;
    }
}

