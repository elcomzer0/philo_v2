/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 10:06:05 by jorgonca          #+#    #+#             */
/*   Updated: 2024/05/19 16:55:10 by codespace        ###   ########.fr       */
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

    write(1, "Exiting...\n", 12);
    if(data->philosophers)
    {
    for (i = 0; i < data->number_of_philosophers; i++)
    {
        if (pthread_mutex_destroy(&data->philosophers[i].left_fork) != 0)
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
}













/* void free_data(t_data *data)
{
    int i;
    
    if (!data)
        return;

    i = 1;
    while(i < data->number_of_philosophers)
    {
    write(1, "Exiting...\n", 12);
        if(pthread_mutex_destroy(&data->philosophers[i].left_fork) != 0)
        {
            write(2, "Error: mutex_destroy\n", 21);
            free(data->philosophers);
            pthread_mutex_destroy(&data->print_lock);
            free(data);
            data = NULL;
            return;
        }
        
        i++;
    }
    if (data->number_of_philosophers == 1)
    {
        if(pthread_mutex_destroy(&data->philosophers[0].left_fork) != 0)
        {
            write(2, "Error: mutex_destroy\n", 21);
            free(data->philosophers);
            pthread_mutex_destroy(&data->print_lock);
            free(data);
            data = NULL;
            return;
        }               
    }
    //free the philosophers array
    free(data->philosophers);
    data->philosophers = NULL;

    //destroy the print mutex
    pthread_mutex_destroy(&data->print_lock);
    //free the data structure itself
    free(data);
    data = NULL;
}

void clean_exit(t_data *data)
{
    int i;
    
    if (!data)
        return; 

    
    i = 0;
    if (data->number_of_philosophers > 1)
    {
        while (i < data->number_of_philosophers)
        {
            if(pthread_join(data->philosophers[i].thread, NULL) != 0)
                write(2, "Error: pthread_join\n", 20);
                
                //printf("Error joining thread %d\n", i + 1);
            //write(1, "Exiting...\n", 12);
        i++;
        }
    }
    else
    {
        //??????????????? like this? why detach and 
       // if(pthread_detach(data->philosophers[0].thread) != 0)
        if(pthread_detach(data->philosophers[0].thread) != 0)
        {
            write(2, "Error: pthread_detach\n", 21);
            exit(EXIT_FAILURE);
        }
        
    }     
    
    free_data(data);
    exit(EXIT_SUCCESS);
} */