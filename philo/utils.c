/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 10:06:05 by jorgonca          #+#    #+#             */
/*   Updated: 2024/06/18 20:53:34 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void *ft_memset(void *b, int c, size_t len)
{
    unsigned char *dst; 
    size_t i;
    
    dst = (unsigned char *)b;
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
    pthread_mutex_lock(&philo->data->death);
    if (!philo->data->death_note || strcmp(status, "died") == 0)
    {
        printf("%ld %d %s\n", get_current_time() - philo->data->start_time, philo->id, status);
    }
    pthread_mutex_unlock(&philo->data->death);
    pthread_mutex_unlock(&philo->data->print_lock);
}

void ft_usleep(unsigned long long int time_value, t_philosopher *philo)
{
    struct timeval start, current;
    unsigned long long elapsed;

    if (philo == NULL || philo->data == NULL)
        return ;
    gettimeofday(&start, NULL);
    while (1)
    {
        gettimeofday(&current, NULL);
        elapsed = (current.tv_sec - start.tv_sec) * 1000 + (current.tv_usec - start.tv_usec) / 1000;
        pthread_mutex_lock(&philo->data->death);
        if (elapsed >= time_value || philo->data->death_note == 1)
        {
            pthread_mutex_unlock(&philo->data->death);
            break ;
        }
        pthread_mutex_unlock(&philo->data->death);
        usleep(400);
    }
}

void free_resources(t_data *data)
{
    if (data->fork)
    {
        free(data->fork);
        data->fork = NULL;
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

void destroy_mutexes(t_data *data)
{
    if (pthread_mutex_destroy(&data->last_meal_timestamps_mutex) != 0)
        write(2, "Error: last_meal_timestamps mutex_destroy\n", 43);
    if (pthread_mutex_destroy(&data->print_lock) != 0)
        write(2, "Error: print mutex_destroy\n", 28);
    if (pthread_mutex_destroy(&data->death) != 0)
        write(2, "Error: death mutex_destroy\n", 27);
    if (pthread_mutex_destroy(&data->dined) != 0)
        write(2, "Error: dined mutex_destroy\n", 27);
    if (pthread_mutex_destroy(&data->dining_mutex) != 0)
        write(2, "Error: dining mutex_destroy\n", 27);
}

void join_threads(t_data *data)
{
    int i;
    
    if (data->number_of_philosophers > 1)
    {
        i = 0;
        while (i < data->number_of_philosophers)
        {
            pthread_join(data->philosophers[i].thread, NULL);
            i++;
        }
    }
    if (data->monitor_eat)
        pthread_join(data->monitor_eat, NULL);
    if (data->monitor_death)
        pthread_join(data->monitor_death, NULL);
}

void clean_exit(t_data *data)
{
    if (!data)
        return;

    join_threads(data);
    destroy_mutexes(data);
    free_resources(data);
}


