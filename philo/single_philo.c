/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_philo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 10:05:31 by jorgonca          #+#    #+#             */
/*   Updated: 2024/05/19 16:55:33 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */




#include "philo.h"



void *sokrates_case(void *arg)
{
    t_data *data = (t_data *)arg;
    usleep((int)data->time_to_die);
    printf("%ld 1 died\n", data->time_to_die);    
    return (NULL);
}

void initialize_single_philosopher_case(t_data *data)
{
    //initialize_single_philosopher(philosopher, 1, data);
    //philosopher->right_fork = NULL;
    //philosopher->next = NULL;
    //philosopher->prev = NULL;
   // printf("Initializing single philosopher case\n");
    create_single_philosopher_thread(data);
}

void create_single_philosopher_thread(t_data *data)
{   
    pthread_t thread;
    
    if (pthread_create(&thread, NULL, &sokrates_case, data) != 0)
    {
        perror("Error creating philosopher thread");
        exit(EXIT_FAILURE);
    }
    pthread_join(thread, NULL);
    //write(1, "halloooo\n", 9);
}

