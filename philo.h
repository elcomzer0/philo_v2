/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 10:06:36 by jorgonca          #+#    #+#             */
/*   Updated: 2024/05/19 18:12:39 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
#define PHILO_H

#include <pthread.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>

#define SIZE_MAX ((size_t)-1)



//perhaps enumeration for a state maching in case of monitoring
/* typedef enum state_monitor {
    
    
    
} state_monitor; */


/**
 * Represents a philosopher in the dining philosophers problem.
 *
 * Each philosopher has an ID, the time of their last meal, the number of meals they have eaten,
 * a thread to represent their actions, a left and right fork, pointers to the next and previous
 * philosophers, and a reference to the overall dining data.
 */
typedef struct s_philosopher
{
    int id;
    long last_meal_time;
    int meals_eaten;
    pthread_t thread;
    pthread_mutex_t left_fork;
    pthread_mutex_t *right_fork;
    struct s_philosopher *next;
    struct s_philosopher *prev;
    struct s_data *data;
} t_philosopher;

typedef struct s_data
{
    int number_of_philosophers;
    long time_to_die;
    long time_to_eat;
    long time_to_sleep;
    int times_must_eat;
    t_philosopher *philosophers; 
    pthread_mutex_t print_lock;
} t_data;

// Function prototypes
void initialize_philosophers(t_data *data);
void start_simulation(t_data *data);
void *philosopher_routine(void *arg);
void ft_usleep(unsigned long long int usec);

//single_philosopher
void *sokrates_case(void *arg);
void initialize_single_philosopher_case(t_data *data);
void create_single_philosopher_thread(t_data *data);

/* utils */
void *ft_calloc(size_t num_elements, size_t element_size);
long get_current_time();
void print_status(t_philosopher *philo, const char *status);
void clean_exit(t_data *data);



#endif
