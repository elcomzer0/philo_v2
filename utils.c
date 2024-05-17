/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 10:06:05 by jorgonca          #+#    #+#             */
/*   Updated: 2024/05/17 14:50:05 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */




#include "philo.h"


long get_current_time()
{
    struct timeval tv;
    unsigned long long time;
    gettimeofday(&tv, NULL);
    time = (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000);
    return (time);
}

void print_status(t_philosopher *philo, const char *status)
{
    pthread_mutex_lock(&philo->data->print_lock);
    printf("%ld %d %s\n", get_current_time(), philo->id, status);
    pthread_mutex_unlock(&philo->data->print_lock);
}

void ft_usleep(unsigned long long int usec)
{
    struct timeval start, current;
    unsigned long long elapsed = 0;

    // Get the start time
    gettimeofday(&start, NULL);

    // Loop until the elapsed time is greater than or equal to the specified time
    while (elapsed < usec)
    {
        // Get the current time
        gettimeofday(&current, NULL);

        // Calculate the elapsed time in microseconds
        elapsed = (current.tv_sec - start.tv_sec) * 1000000ULL + (current.tv_usec - start.tv_usec);
    }
}

