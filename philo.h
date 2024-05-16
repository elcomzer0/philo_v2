/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 10:06:36 by jorgonca          #+#    #+#             */
/*   Updated: 2024/05/16 10:06:53 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
#define PHILO_H

#include <pthread.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

// Philosopher structure representing each node in the tree
typedef struct s_philo
{
    int id;
    int meals_eaten;
    long last_meal;
    pthread_t thread;
    pthread_mutex_t left_fork;
    pthread_mutex_t right_fork;
    struct s_philo *left;
    struct s_philo *right;
    struct s_data *data;
} t_philo;

// Global data structure for the simulation
typedef struct s_data
{
    int num_philos;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int num_meals;
    long start_time;
    pthread_mutex_t print_mutex;
    t_philo *root;
} t_data;

int init_data(t_data *data, int argc, char **argv);
t_philo *create_philosopher_tree(int id, int max_id, t_data *data);
void init_philosophers(t_philo *root);
void *philosopher_routine(void *arg);
void *monitor_routine(void *arg);
long get_timestamp();
void print_status(t_philo *philo, const char *status);
void start_philosopher_threads(t_philo *philo);
void join_philosopher_threads(t_philo *philo);
void destroy_philosopher_tree(t_philo *philo);

#endif