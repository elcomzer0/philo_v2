/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_philo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 10:05:31 by jorgonca          #+#    #+#             */
/*   Updated: 2024/05/27 23:18:58 by jorgonca         ###   ########.fr       */
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
void create_single_philosopher_thread(t_data *data)
{   
    pthread_t thread;
    
    if (pthread_create(&thread, NULL, &sokrates_case, data) != 0)
    {
        perror("Error creating philosopher thread");
        exit(EXIT_FAILURE);
    }
    pthread_join(thread, NULL);
}

void initialize_single_philosopher_case(t_data *data)
{
    create_single_philosopher_thread(data);
}


