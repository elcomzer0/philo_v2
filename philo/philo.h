/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 10:06:36 by jorgonca          #+#    #+#             */
/*   Updated: 2024/06/18 20:54:43 by jorgonca         ###   ########.fr       */
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
#include <time.h>
#include <limits.h>

#define SIZE_MAX ((size_t)-1)
#define MAX_PHILOSOPHERS 201

typedef struct s_philosopher
{
    int id;
    long last_meal_time;
    int meals_eaten;
    long time_to_die;
    pthread_t thread;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    struct s_data *data;
} t_philosopher;

typedef struct s_data
{
    long start_time;
    int number_of_philosophers;
    long time_to_die;
    long time_to_eat;
    long time_to_sleep;
    int times_must_eat;
    int death_note;
    int dined_enough;
    long long last_meal_timestamps[201];
    pthread_t monitor_eat;
    pthread_t monitor_death;
    pthread_mutex_t *fork;
    pthread_mutex_t print_lock;
    pthread_mutex_t death;
    pthread_mutex_t dined;
    pthread_mutex_t meals_eaten_mutex;
    pthread_mutex_t last_meal_timestamps_mutex;
    pthread_mutex_t dining_mutex;
    t_philosopher *philosophers; 
} t_data;

// Function prototypes
void initialize_philosophers(t_data *data);
void start_simulation(t_data *data);
void *philosopher_routine(void *arg);
void ft_usleep(unsigned long long int time_value, t_philosopher *philo);

//single_philosopher
void *sokrates_case(void *arg);
void initialize_single_philosopher_case(t_data *data);
void create_single_philosopher_thread(t_data *data);

/* utils */
void *ft_calloc(size_t num_elements, size_t element_size);
long get_current_time();
void print_status(t_philosopher *philo, const char *status);
void clean_exit(t_data *data);
//bool is_death_note(t_philosopher *philo);

/*monitor*/
void *monitor_eat(void *arg);
void *monitor_death(void *arg);
int death_note_check(t_philosopher *philo);

/*actions*/
void action_eat(t_philosopher *philo);
void action_sleep(t_philosopher *philo);
void action_think(t_philosopher *philo);
void print_philosopher(t_philosopher *philosopher);
void print_data(const t_data *data);
void random_delay(int min_ms, int max_ms);

#endif
