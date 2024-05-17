/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 10:05:31 by jorgonca          #+#    #+#             */
/*   Updated: 2024/05/17 14:49:09 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */




#include "philo.h"

/* one philo case. if one  */
/* void sokrates_case(t_philosopher *philo)
{
    
} */



void initialize_philosophers(t_data *data)
{
    t_philosopher *philosophers = malloc(sizeof(t_philosopher) * data->number_of_philosophers);
    if (!philosophers)
        exit(EXIT_FAILURE);

    int i = 0;
    while( i < data->number_of_philosophers)
    {
        philosophers[i].id = i + 1;
        philosophers[i].last_meal_time = get_current_time();
        philosophers[i].meals_eaten = 0;
        pthread_mutex_init(&philosophers[i].left_fork, NULL);

        if (i == data->number_of_philosophers - 1)
            philosophers[i].right_fork = &philosophers[0].left_fork;
        else
            philosophers[i].right_fork = &philosophers[i + 1].left_fork;

        philosophers[i].next = &philosophers[(i + 1) % data->number_of_philosophers];
        philosophers[i].prev = &philosophers[(i - 1 + data->number_of_philosophers) % data->number_of_philosophers];
    i++;
    }
    data->philosophers = philosophers;
}

int main(int argc, char **argv)
{
    if (argc != 5 && argc != 6)
    {
        printf("Usage: %s number_of_philosophers time_to_die time_to_eat time_to_sleep [times_must_eat]\n", argv[0]);
        return 1;
    }

    t_data data;
    data.number_of_philosophers = atoi(argv[1]);
    data.time_to_die = atol(argv[2]);
    data.time_to_eat = atol(argv[3]);
    data.time_to_sleep = atol(argv[4]);
    if (argc == 6)
    {
        data.times_must_eat = atoi(argv[5]);
    }
    else
    {
        data.times_must_eat = -1;
    }

    initialize_philosophers(&data);

    start_simulation(&data);

    free(data.philosophers);
    return 0;
}

