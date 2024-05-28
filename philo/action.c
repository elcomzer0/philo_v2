/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 18:21:10 by jorgonca          #+#    #+#             */
/*   Updated: 2024/05/28 16:56:44 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
//#include <pthread.h>
#include <stdint.h>

static uint32_t seed = 1;
pthread_mutex_t rand_mutex = PTHREAD_MUTEX_INITIALIZER;

void srand_custom(uint32_t new_seed) {
    pthread_mutex_lock(&rand_mutex);
    seed = new_seed;
    pthread_mutex_unlock(&rand_mutex);
}

uint32_t rand_custom() {
    pthread_mutex_lock(&rand_mutex);
    seed = (1103515245 * seed + 12345) % (1 << 31);
    uint32_t result = seed;
    pthread_mutex_unlock(&rand_mutex);
    return result;
}

int rand_range(int min, int max) {
    return min + rand_custom() % (max - min + 1);
}

void random_delay(int min_ms, int max_ms)
{
    int delay = rand_range(min_ms, max_ms);
    usleep(delay * 1000);
    //ft_usleep(delay * 1000);
}

void random_delay_v2(int min_ms, int max_ms, t_philosopher *philo) {
    int delay = rand_range(min_ms, max_ms);
   // usleep(delay * 1000);
    ft_usleep(delay, philo);
}

int    ft_abs(int v)
{
    if (v < 0)
        return (-v);
    return (v);
}


void update_last_meal_time(t_philosopher *philo)
{
    pthread_mutex_lock(&philo->data->last_meal_timestamps_mutex);
    philo->last_meal_time = get_current_time();
    philo->data->last_meal_timestamps[philo->id - 1] = philo->last_meal_time;
    pthread_mutex_unlock(&philo->data->last_meal_timestamps_mutex);
}




void release_forks(t_philosopher *philo)
{
    pthread_mutex_unlock(philo->left_fork);
    pthread_mutex_unlock(philo->right_fork);
    pthread_mutex_lock(&philo->data->fork_status_mutex);
    *(philo->l_fork) = 0;
    *(philo->r_fork) = 0;
    pthread_mutex_unlock(&philo->data->fork_status_mutex);
}



void acquire_forks(t_philosopher *philo)
{
   // usleep(100);
   random_delay(10, 150);
    pthread_mutex_lock(&philo->data->dining_mutex);
    pthread_mutex_lock(&philo->data->death);
    if (philo->data->death_note) {
        pthread_mutex_unlock(&philo->data->death);
        pthread_mutex_unlock(&philo->data->dining_mutex);
        return;
    }
    pthread_mutex_unlock(&philo->data->death);

    pthread_mutex_lock(philo->left_fork);
    pthread_mutex_lock(philo->right_fork);

    pthread_mutex_lock(&philo->data->fork_status_mutex);
    *(philo->l_fork) = 1;
    *(philo->r_fork) = 1;
    pthread_mutex_unlock(&philo->data->fork_status_mutex);

    print_status(philo, "has taken a fork");
    print_status(philo, "has taken a fork");

    pthread_mutex_unlock(&philo->data->dining_mutex);
}


void action_eat(t_philosopher *philo)
{
    acquire_forks(philo);
    update_last_meal_time(philo);
    //random_delay_v2(100, 200, philo);
    //random_delay(100, 200);
    print_status(philo, "is eating");
    release_forks(philo);
}



void action_sleep(t_philosopher *philo)
{
    print_status(philo, "is sleeping");
   // ft_usleep(philo->data->time_to_sleep , philo);
    random_delay(50, 150);  // Add a random delay to prevent immediate fork contention
    //printf("amount of time a philosophers sleep is: %ld\n", get_current_time() - philo->last_meal_time);
}

/* void action_think(t_philosopher *philo)
{
    print_status(philo, "is thinking");
    if ((philo->id % 2)
        && (philo->data->time_to_sleep != philo->data->time_to_eat))
        ft_usleep(ft_abs((philo->data->time_to_sleep
                - philo->data->time_to_eat) * 1000 + 150), philo);
   // printf("amount of time a philosophers think is: %ld\n", get_current_time() - philo->last_meal_time);
} */

void action_think(t_philosopher *philo) {
    print_status(philo, "is thinking");
    //random_delay(100, 300);  // Add a random delay to prevent immediate fork contention
    random_delay_v2(100, 300, philo);  // Add a random delay to prevent immediate fork contention
}

/* void action_think(t_philosopher *philo)
{
    
    print_status(philo, "is thinking");
    //usleep(100); // Sleep for a short time to avoid busy waiting
} */
