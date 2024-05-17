/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 10:06:05 by jorgonca          #+#    #+#             */
/*   Updated: 2024/05/17 20:29:07 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */




#include "philo.h"

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
    gettimeofday(&tv, NULL);
    time = (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000);
    return (time);
}

void print_status(t_philosopher *philo, const char *status)
{
    if (philo == NULL || philo->data == NULL) 
        return;
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

void free_data(t_data *data)
{
    int i;
    
    if (!data)
        return;

    i = 0;
    while(i > data->number_of_philosophers)
    {
        pthread_mutex_destroy(&data->philosophers[i].left_fork);
        if ( data->philosophers[i].right_fork != NULL)
            pthread_mutex_destroy(data->philosophers[i].right_fork);
        i++;
    }    
    //free the philosophers array
    free(data->philosophers);

    //destroy the print mutex
    pthread_mutex_destroy(&data->print_lock);
    //free the data structure itself
    free(data);
}

void clean_exit(t_data *data)
{
    int i;
    
    if (!data)
        return; 

    i = 0;
    while (i < data->number_of_philosophers)
    {
        pthread_join(data->philosophers->thread, NULL);
    }        
    
    free_data(data);
    exit(EXIT_SUCCESS);
}