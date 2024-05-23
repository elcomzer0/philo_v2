/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 10:06:05 by jorgonca          #+#    #+#             */
/*   Updated: 2024/05/23 17:06:46 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */




#include "philo.h"


bool is_death_note(t_philosopher *philo)
{
    if (philo->data->times_must_eat != -1 && philo->meals_eaten >= philo->data->times_must_eat)
        return (true);
    return (false);
}

void *ft_memset(void *b, int c, size_t len)
{
    unsigned char *dst = (unsigned char *)b;
    size_t i;
    
    i = 0;
    while (i < len)
    {
        dst[i] = (unsigned char)c;
        i++;
    }
    return (b);
}

void *ft_calloc(size_t num_elements, size_t element_size)
{
    size_t total_size;
    void *ptr;

    if (num_elements && SIZE_MAX / num_elements < element_size)
        return (NULL);
    total_size = num_elements * element_size;
    ptr = malloc(total_size);
    if (!ptr)
        return (NULL);
    if(ptr)
        memset(ptr, 0, total_size);
    return (ptr);
}

long get_current_time()
{
    struct timeval tv;
    unsigned long long time;
    if (gettimeofday(&tv, NULL) != 0)
        return (-1);
    time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
    return (time);
}

void print_status(t_philosopher *philo, const char *status)
{
    if (philo == NULL || philo->data == NULL) 
        return;
    pthread_mutex_lock(&philo->data->print_lock);
    printf("%ld %d %s\n", get_current_time() - philo->data->start_time, philo->id, status);
    pthread_mutex_unlock(&philo->data->print_lock);
}

void ft_usleep(unsigned long long int time_value, t_philosopher *philo)
{
    struct timeval start, current;
    unsigned long long elapsed;

    // Get the start time
    gettimeofday(&start, NULL);

    // Loop until the elapsed time is greater than or equal to the specified time
    while (1)
    {
        // Get the current time
        gettimeofday(&current, NULL);

        // Calculate the elapsed time in microseconds
        //elapsed = (current.tv_sec - start.tv_sec) * 1000000ULL + (current.tv_usec - start.tv_usec);
        elapsed = (current.tv_sec - start.tv_sec) * 1000 + (current.tv_usec - start.tv_usec) / 1000;
        pthread_mutex_lock(&philo->data->death);
        if(elapsed >= time_value || philo->data->death_note == 1)
        {
            pthread_mutex_unlock(&philo->data->death);
            break ;
        }
        usleep(400);
        pthread_mutex_unlock(&philo->data->death);
    }
}

void free_data(t_data *data)
{
    int i;

    if (!data)
        return;

    write(1, "Exiting...\n", 12);
    if(data->philosophers)
    {
    for (i = 0; i < data->number_of_philosophers; i++)
    {
        if (pthread_mutex_destroy(data->philosophers[i].left_fork) != 0)
        {
            write(2, "Error: mutex_destroy\n", 21);
        }
    }
    }
    if (pthread_mutex_destroy(&data->print_lock) != 0)
    {
        write(2, "Error: print mutex_destroy\n", 28);
    }

    if (data->philosophers)
    {
        free(data->philosophers);
        data->philosophers = NULL;
    }
    if (data)
    {
        free(data);
        data = NULL;
}
}

void clean_exit(t_data *data)
{

    if (!data)
        return;

    write(1, "Exiting...\n", 12);
   
    if(data->philosophers)
    {
    for (int i = 0; i < data->number_of_philosophers; i++)
    {
        if (pthread_mutex_destroy(data->philosophers[i].left_fork) != 0)
        {
            write(2, "Error: mutex_destroy\n", 21);
        }
    }
    for (int i = 0; i < data->number_of_philosophers; i++)
    {
        if (pthread_mutex_destroy(data->philosophers[i].right_fork) != 0)
        {
            write(2, "Error: mutex_destroy\n", 21);
        }
    }
    
    }
    if (pthread_mutex_destroy(&data->print_lock) != 0)
    {
        write(2, "Error: print mutex_destroy\n", 28);
    }
    if (pthread_mutex_destroy(&data->death) != 0)
    {
        write(2, "Error: death mutex_destroy\n", 27);
    }

    if (pthread_mutex_destroy(&data->dined) != 0)
    {
        write(2, "Error: dined mutex_destroy\n", 27);
    }
   if (data->philosophers)
    {
        free(data->philosophers);
        data->philosophers = NULL;
    }
    if (data->forks)
    {
        for (int i = 0; i < data->number_of_philosophers; i++)
        {
            pthread_mutex_destroy(&data->fork[i]);
        }
        free(data->forks);
        data->forks = NULL;
    }
    if (data->fork)
    {
        free(data->fork);
        data->fork = NULL;
    }
    if (data)
    {
        free(data);
        data = NULL;
    }
    exit(EXIT_SUCCESS);
}

/* void clean_exit(t_data *data)
{
    int i;

    if (!data)
        return;

    write(1, "Exiting...\n", 12);
   
    if (data->number_of_philosophers > 1)
    {
        for (i = 0; i < data->number_of_philosophers; i++)
        {
            if (pthread_join(data->philosophers[i].thread, NULL) != 0)
                write(2, "Error: pthread_join\n", 20);
        }
    }
    else
    {
        free(data);
    }
    exit(EXIT_SUCCESS);
} */