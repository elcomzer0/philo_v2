/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 18:21:10 by jorgonca          #+#    #+#             */
/*   Updated: 2024/06/17 22:35:56 by jorgonca         ###   ########.fr       */
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

/* 
    This function is used to update the last meal time of a philosopher.
    It is called when a philosopher finishes eating.
    It locks the last meal timestamps mutex, updates the philosopher's last meal time,
    and then unlocks the last meal timestamps mutex.
    
*/
void update_last_meal_time(t_philosopher *philo)
{
    pthread_mutex_lock(&philo->data->last_meal_timestamps_mutex);
    philo->last_meal_time = get_current_time(); // Update the last meal time of the philosopher
    philo->data->last_meal_timestamps[philo->id - 1] = philo->last_meal_time; // Update the last meal time in the array of last meal times. 

    pthread_mutex_unlock(&philo->data->last_meal_timestamps_mutex);
}



/*
    This function is used to release the forks of a philosopher.
    It locks the fork status mutex, sets the left and right fork variables to 0,
    and then unlocks the fork status mutex.
    It then unlocks the left and right forks.
*/
void release_forks(t_philosopher *philo)
{
    pthread_mutex_unlock(philo->left_fork);
    pthread_mutex_unlock(philo->right_fork);
  /*   pthread_mutex_lock(&philo->data->fork_status_mutex);
    *(philo->l_fork) = 0;

    *(philo->r_fork) = 0;
    pthread_mutex_unlock(&philo->data->fork_status_mutex); */
}



// void acquire_forks(t_philosopher *philo)
// {
 
//     pthread_mutex_lock(&philo->data->dining_mutex);
  
//  // Lock the dining mutex to prevent other philosophers from eating
//     pthread_mutex_lock(&philo->data->death); // Lock the death mutex to prevent the philosopher from eating if they are about to die
//     if (philo->data->death_note) // if the philosopher has a death note, then release the forks and return
//     {
//         pthread_mutex_unlock(&philo->data->death);
//         pthread_mutex_unlock(&philo->data->dining_mutex);
//         return;
//     }
//     pthread_mutex_unlock(&philo->data->death);

//     pthread_mutex_lock(philo->left_fork);
//     pthread_mutex_lock(philo->right_fork);

//   /*   pthread_mutex_lock(&philo->data->fork_status_mutex);

//     *(philo->l_fork) = 1;

//     *(philo->r_fork) = 1;
//     pthread_mutex_unlock(&philo->data->fork_status_mutex); */

//     print_status(philo, "has taken a fork");
//     print_status(philo, "has taken a fork");

//     pthread_mutex_unlock(&philo->data->dining_mutex);
// }

void acquire_forks(t_philosopher *philo)
{
 
    pthread_mutex_lock(&philo->data->dining_mutex);
  
 // Lock the dining mutex to prevent other philosophers from eating
    pthread_mutex_lock(&philo->data->death); // Lock the death mutex to prevent the philosopher from eating if they are about to die
    if (philo->data->death_note) // if the philosopher has a death note, then release the forks and return
    {
        pthread_mutex_unlock(&philo->data->death);
        pthread_mutex_unlock(&philo->data->dining_mutex);
        return;
    }
    pthread_mutex_unlock(&philo->data->death);

    pthread_mutex_lock(philo->left_fork);
    pthread_mutex_lock(philo->right_fork);

  /*   pthread_mutex_lock(&philo->data->fork_status_mutex);

    *(philo->l_fork) = 1;

    *(philo->r_fork) = 1;
    pthread_mutex_unlock(&philo->data->fork_status_mutex); */

    print_status(philo, "has taken a fork");
    print_status(philo, "has taken a fork");

    pthread_mutex_unlock(&philo->data->dining_mutex);
}


void action_eat(t_philosopher *philo)
{
    acquire_forks(philo);
    update_last_meal_time(philo);
    //usleep(100);
   //usleep(philo->data->time_to_eat);
/*     if (philo->id % 2 == 0)
       ft_usleep(philo->data->time_to_eat, philo);
    else */
        ft_usleep(philo->data->time_to_eat, philo);
    print_status(philo, "is eating");
    release_forks(philo);
}


void action_sleep(t_philosopher *philo)
{
    print_status(philo, "is sleeping");
    ft_usleep(philo->data->time_to_sleep, philo);
    //random_delay(50, 150);  // Add a random delay to prevent immediate fork contention
    //printf("amount of time a philosophers sleep is: %ld\n", get_current_time() - philo->last_meal_time);
}

/* void action_think(t_philosopher *philo)
{
    print_status(philo, "is thinking");
   // usleep(100); // Sleep for a short time to avoid busy waiting
    //if ((philo->id % 2) && (philo->data->time_to_sleep != philo->data->time_to_eat)) // If the philosopher is even and the time to sleep is not the same as the time to eat, then sleep for a random amount of time between the time to sleep and the time to eat
      //random_delay(100, 300);  // Add a random delay to prevent immediate fork contention
  //  random_delay_v2(100, 300, philo);  // Add a random delay to prevent immediate fork contention
} */


/* void action_think(t_philosopher *philo)
{
    print_status(philo, "is thinking");
    if ((philo->id % 2) 
       && (philo->data->time_to_sleep != philo->data->time_to_eat)) // If the philosopher is even and the time to sleep is not the same as the time to eat, then sleep for a random amount of time between the time to sleep and the time to eat
        ft_usleep(ft_abs((philo->data->time_to_sleep
                - philo->data->time_to_eat) * 1000 + 150), philo);
   // printf("amount of time a philosophers think is: %ld\n", get_current_time() - philo->last_meal_time);
} */


void action_think(t_philosopher *philo)
{
    
    print_status(philo, "is thinking");
    //usleep(100); // Sleep for a short time to avoid busy waiting
}
