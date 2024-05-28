/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 18:21:10 by jorgonca          #+#    #+#             */
/*   Updated: 2024/05/28 15:50:13 by jorgonca         ###   ########.fr       */
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

/* void acquire_forks(t_philosopher *philo, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
{
    while (1)
    {
        pthread_mutex_lock(&philo->data->fork_status_mutex);
        if (*(philo->l_fork) == 0 && *(philo->r_fork) == 0) {
            pthread_mutex_lock(first_fork);
            pthread_mutex_lock(second_fork);
            *(philo->l_fork) = 1;
            *(philo->r_fork) = 1;
            pthread_mutex_unlock(&philo->data->fork_status_mutex);
            print_status(philo, "has taken a fork");
            print_status(philo, "has taken a fork");
            philo->starvation_counter = 0; // Reset the starvation counter
            break;
        } else {
            pthread_mutex_unlock(&philo->data->fork_status_mutex);
            philo->starvation_counter++; // Increment the starvation counter
           // ft_usleep(400, philo); // Sleep for a short time to avoid busy waiting
            //random_delay(100, 150); // Sleep for a random time to avoid busy waiting
            usleep(100); // Sleep for a short time to avoid busy waiting
        }
    }
} */




/* void acquire_forks(t_philosopher *philo, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
{
    pthread_mutex_lock(&philo->data->dining_mutex);
    while (1)
    {
        pthread_mutex_lock(&philo->data->fork_status_mutex);
        if (*(philo->l_fork) == 0 && *(philo->r_fork) == 0) {
            if (philo->id % 2 == 0) {
                usleep(100);
                pthread_mutex_lock(first_fork);
                pthread_mutex_lock(second_fork);
            } else {
                usleep(100);
                pthread_mutex_lock(second_fork);
                pthread_mutex_lock(first_fork);
            }
            *(philo->l_fork) = 1;
            *(philo->r_fork) = 1;
            pthread_mutex_unlock(&philo->data->fork_status_mutex);
            print_status(philo, "has taken a fork");
            print_status(philo, "has taken a fork");
            philo->starvation_counter = 0; // Reset the starvation counter
            break;
        } else {
            pthread_mutex_unlock(&philo->data->fork_status_mutex);
            philo->starvation_counter++; // Increment the starvation counter
            usleep(100); // Sleep for a short time to avoid busy waiting
        }
    }
        pthread_mutex_unlock(&philo->data->dining_mutex); // release dining mutex last

} */






/* void release_forks(t_philosopher *philo, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
{
    pthread_mutex_lock(&philo->data->dining_mutex);
    pthread_mutex_lock(&philo->data->fork_status_mutex);
    *(philo->l_fork) = 0;
    *(philo->r_fork) = 0;
    pthread_mutex_unlock(&philo->data->fork_status_mutex);
    pthread_mutex_unlock(first_fork);
    pthread_mutex_unlock(second_fork);
        pthread_mutex_unlock(&philo->data->dining_mutex); // release dining mutex last

} */


/* void acquire_forks_even(t_philosopher *philo)
{
    pthread_mutex_lock(&philo->data->dining_mutex);
    pthread_mutex_lock(&philo->data->fork_status_mutex);
    while (*(philo->l_fork) != 0 || *(philo->r_fork) != 0) {
        pthread_mutex_unlock(&philo->data->fork_status_mutex);
        usleep(100);
        pthread_mutex_lock(&philo->data->fork_status_mutex);
    }
    *(philo->l_fork) = 1;
    *(philo->r_fork) = 1;
    pthread_mutex_unlock(&philo->data->fork_status_mutex);
    pthread_mutex_lock(philo->left_fork);
    pthread_mutex_lock(philo->right_fork);
    pthread_mutex_unlock(&philo->data->dining_mutex);
    print_status(philo, "has taken a fork");
    print_status(philo, "has taken a fork");
} */

/* void release_forks_even(t_philosopher *philo)
{
    pthread_mutex_lock(&philo->data->dining_mutex);
    pthread_mutex_unlock(philo->left_fork);
    pthread_mutex_unlock(philo->right_fork);
    pthread_mutex_lock(&philo->data->fork_status_mutex);
    *(philo->l_fork) = 0;
    *(philo->r_fork) = 0;
    pthread_mutex_unlock(&philo->data->fork_status_mutex);
    pthread_mutex_unlock(&philo->data->dining_mutex);
} */
//////////////////////
/* void release_forks_even(t_philosopher *philo)
{
    pthread_mutex_lock(&philo->data->dining_mutex);
    pthread_mutex_unlock(philo->right_fork);
    pthread_mutex_lock(&philo->data->fork_status_mutex);
    *(philo->r_fork) = 0;
    pthread_mutex_unlock(&philo->data->fork_status_mutex);
    pthread_mutex_unlock(philo->left_fork);
    pthread_mutex_lock(&philo->data->fork_status_mutex);
    *(philo->l_fork) = 0;
    pthread_mutex_unlock(&philo->data->fork_status_mutex);
    pthread_mutex_unlock(&philo->data->dining_mutex);
}
void acquire_forks_even(t_philosopher *philo)
{
    pthread_mutex_lock(&philo->data->dining_mutex);
    pthread_mutex_lock(philo->left_fork);
    pthread_mutex_lock(&philo->data->fork_status_mutex);
    *(philo->l_fork) = 1;
    pthread_mutex_unlock(&philo->data->fork_status_mutex);
    pthread_mutex_lock(&philo->data->death);
    if (philo->data->death_note) {
        pthread_mutex_unlock(&philo->data->death);
        release_forks_even(philo);
        return;
    }
    pthread_mutex_unlock(&philo->data->death);
    print_status(philo, "has taken a fork");
    pthread_mutex_lock(philo->right_fork);
    pthread_mutex_lock(&philo->data->fork_status_mutex);
    *(philo->r_fork) = 1;
    pthread_mutex_unlock(&philo->data->fork_status_mutex);
    pthread_mutex_lock(&philo->data->death);
    if (philo->data->death_note) {
        pthread_mutex_unlock(&philo->data->death);
        release_forks_even(philo);
        return;
    }
    pthread_mutex_unlock(&philo->data->death);
    print_status(philo, "has taken a fork");
    pthread_mutex_unlock(&philo->data->dining_mutex);
} */

//////////////////

/* void acquire_forks_even(t_philosopher *philo)
{
    pthread_mutex_lock(&philo->data->dining_mutex);
    pthread_mutex_lock(&philo->data->fork_status_mutex);
    while (*(philo->l_fork) != 0 || *(philo->r_fork) != 0) {
        pthread_mutex_unlock(&philo->data->fork_status_mutex);
        pthread_mutex_unlock(&philo->data->dining_mutex);
        usleep(100);
        pthread_mutex_lock(&philo->data->dining_mutex);
        pthread_mutex_lock(&philo->data->fork_status_mutex);
    }
    *(philo->l_fork) = 1;
    *(philo->r_fork) = 1;
    pthread_mutex_unlock(&philo->data->fork_status_mutex);
    pthread_mutex_lock(philo->left_fork);
    pthread_mutex_lock(philo->right_fork);
    pthread_mutex_unlock(&philo->data->dining_mutex);
    print_status(philo, "has taken a fork");
    print_status(philo, "has taken a fork");
} */



/* void acquire_forks_odd(t_philosopher *philo)
{
    pthread_mutex_lock(&philo->data->dining_mutex);
    pthread_mutex_lock(&philo->data->fork_status_mutex);
    while (*(philo->l_fork) != 0 || *(philo->r_fork) != 0) {
        pthread_mutex_unlock(&philo->data->fork_status_mutex);
        usleep(100);
        pthread_mutex_lock(&philo->data->fork_status_mutex);
    }
    *(philo->l_fork) = 1;
    *(philo->r_fork) = 1;
    pthread_mutex_unlock(&philo->data->fork_status_mutex);
    pthread_mutex_lock(philo->right_fork);
    pthread_mutex_lock(philo->left_fork);
    pthread_mutex_unlock(&philo->data->dining_mutex);
    print_status(philo, "has taken a fork");
    print_status(philo, "has taken a fork");
}
void release_forks_odd(t_philosopher *philo)
{
    pthread_mutex_lock(&philo->data->dining_mutex);
    pthread_mutex_unlock(philo->right_fork);
    pthread_mutex_unlock(philo->left_fork);
    pthread_mutex_lock(&philo->data->fork_status_mutex);
    *(philo->l_fork) = 0;
    *(philo->r_fork) = 0;
    pthread_mutex_unlock(&philo->data->fork_status_mutex);
    pthread_mutex_unlock(&philo->data->dining_mutex);
} */

//////////////////////////
/* void release_forks_odd(t_philosopher *philo)
{
    pthread_mutex_lock(&philo->data->dining_mutex);
    pthread_mutex_unlock(philo->left_fork);
    pthread_mutex_lock(&philo->data->fork_status_mutex);
    *(philo->l_fork) = 0;
    pthread_mutex_unlock(&philo->data->fork_status_mutex);
    pthread_mutex_unlock(philo->right_fork);
    pthread_mutex_lock(&philo->data->fork_status_mutex);
    *(philo->r_fork) = 0;
    pthread_mutex_unlock(&philo->data->fork_status_mutex);
    pthread_mutex_unlock(&philo->data->dining_mutex);
}
void acquire_forks_odd(t_philosopher *philo)
{
    pthread_mutex_lock(&philo->data->dining_mutex);
    pthread_mutex_lock(philo->right_fork);
    pthread_mutex_lock(&philo->data->fork_status_mutex);
    *(philo->r_fork) = 1;
    pthread_mutex_unlock(&philo->data->fork_status_mutex);
    pthread_mutex_lock(&philo->data->death);
    if (philo->data->death_note) {
        pthread_mutex_unlock(&philo->data->death);
        release_forks_odd(philo);
        return;
    }
    pthread_mutex_unlock(&philo->data->death);
    print_status(philo, "has taken a fork");
    pthread_mutex_lock(philo->left_fork);
    pthread_mutex_lock(&philo->data->fork_status_mutex);
    *(philo->l_fork) = 1;
    pthread_mutex_unlock(&philo->data->fork_status_mutex);
    pthread_mutex_lock(&philo->data->death);
    if (philo->data->death_note) {
        pthread_mutex_unlock(&philo->data->death);
        release_forks_odd(philo);
        return;
    }
    pthread_mutex_unlock(&philo->data->death);
    print_status(philo, "has taken a fork");
    pthread_mutex_unlock(&philo->data->dining_mutex);
} */

//////////////////

/* void acquire_forks_odd(t_philosopher *philo)
{
    pthread_mutex_lock(&philo->data->dining_mutex);
    pthread_mutex_lock(&philo->data->fork_status_mutex);
    while (*(philo->l_fork) != 0 || *(philo->r_fork) != 0) {
        pthread_mutex_unlock(&philo->data->fork_status_mutex);
        pthread_mutex_unlock(&philo->data->dining_mutex);
        usleep(100);
        pthread_mutex_lock(&philo->data->dining_mutex);
        pthread_mutex_lock(&philo->data->fork_status_mutex);
    }
    *(philo->l_fork) = 1;
    *(philo->r_fork) = 1;
    pthread_mutex_unlock(&philo->data->fork_status_mutex);
    pthread_mutex_lock(philo->left_fork);
    pthread_mutex_lock(philo->right_fork);
    pthread_mutex_unlock(&philo->data->dining_mutex);
    print_status(philo, "has taken a fork");
    print_status(philo, "has taken a fork");
} */

/* void release_forks_odd(t_philosopher *philo)
{
    pthread_mutex_lock(&philo->data->dining_mutex);
    pthread_mutex_unlock(philo->left_fork);
    pthread_mutex_unlock(philo->right_fork);
    pthread_mutex_lock(&philo->data->fork_status_mutex);
    *(philo->l_fork) = 0;
    *(philo->r_fork) = 0;
    pthread_mutex_unlock(&philo->data->fork_status_mutex);
    pthread_mutex_unlock(&philo->data->dining_mutex);
} */


/* void release_forks_even(t_philosopher *philo)
{
    pthread_mutex_lock(&philo->data->dining_mutex);
    pthread_mutex_unlock(philo->left_fork);
    pthread_mutex_unlock(philo->right_fork);
    pthread_mutex_lock(&philo->data->fork_status_mutex);
    *(philo->l_fork) = 0;
    *(philo->r_fork) = 0;
    pthread_mutex_unlock(&philo->data->fork_status_mutex);
    pthread_mutex_unlock(&philo->data->dining_mutex);
} */

/* void release_forks(t_philosopher *philo)
{
    pthread_mutex_lock(&philo->data->dining_mutex);
    pthread_mutex_lock(&philo->data->fork_status_mutex);
    *(philo->l_fork) = 0;
    *(philo->r_fork) = 0;
    pthread_mutex_unlock(&philo->data->fork_status_mutex);
    pthread_mutex_unlock(philo->left_fork);
    pthread_mutex_unlock(philo->right_fork);
    pthread_mutex_unlock(&philo->data->dining_mutex);
} */

/////////////////////
void update_last_meal_time(t_philosopher *philo)
{
    pthread_mutex_lock(&philo->data->last_meal_timestamps_mutex);
    philo->last_meal_time = get_current_time();
    philo->data->last_meal_timestamps[philo->id - 1] = philo->last_meal_time;
    pthread_mutex_unlock(&philo->data->last_meal_timestamps_mutex);
}/////////////


/* void action_eat(t_philosopher *philo)
{
    if (philo->id % 2 == 0) {
        acquire_forks_even(philo);
    } else {
        acquire_forks_odd(philo);
    }

    update_last_meal_time(philo);
    print_status(philo, "is eating");

    release_forks(philo);
} */


void release_forks(t_philosopher *philo)
{
    pthread_mutex_unlock(philo->left_fork);
    pthread_mutex_unlock(philo->right_fork);
    pthread_mutex_lock(&philo->data->fork_status_mutex);
    *(philo->l_fork) = 0;
    *(philo->r_fork) = 0;
    pthread_mutex_unlock(&philo->data->fork_status_mutex);
}

/* void acquire_forks(t_philosopher *philo)
{
    pthread_mutex_lock(&philo->data->dining_mutex);
    pthread_mutex_lock(&philo->data->death);
    if (philo->data->death_note) {
        pthread_mutex_unlock(&philo->data->death);
        pthread_mutex_unlock(&philo->data->dining_mutex);
        return;
    }
    pthread_mutex_unlock(&philo->data->death);

    if (philo->id % 2 == 0) {
        pthread_mutex_lock(philo->left_fork);
        pthread_mutex_lock(philo->right_fork);
    } else {
        pthread_mutex_lock(philo->right_fork);
        pthread_mutex_lock(philo->left_fork);
    }

    pthread_mutex_lock(&philo->data->fork_status_mutex);
    *(philo->l_fork) = 1;
    *(philo->r_fork) = 1;
    pthread_mutex_unlock(&philo->data->fork_status_mutex);

    print_status(philo, "has taken a fork");
    print_status(philo, "has taken a fork");

    pthread_mutex_unlock(&philo->data->dining_mutex);
} */

void acquire_forks(t_philosopher *philo)
{
    usleep(100);
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
    print_status(philo, "is eating");
    release_forks(philo);
}


///////////////
/* void action_eat(t_philosopher *philo)
{
    if (philo->id % 2 == 0) {
        acquire_forks_even(philo);
        update_last_meal_time(philo);
        print_status(philo, "is eating");
        release_forks_even(philo);
    } else {
        acquire_forks_odd(philo);
        update_last_meal_time(philo);
        print_status(philo, "is eating");
        release_forks_odd(philo);
    }
} */
/////////////////////


/* void action_eat(t_philosopher *philo)
{
    pthread_mutex_t *first_fork, *second_fork;
  
    if (philo->id % 2 == 0) {
        // Even philosophers acquire left fork first, then right fork
       // usleep(100);
        //random_delay(50, 100);
        first_fork = philo->right_fork;
        second_fork = philo->left_fork;
    } else {
        // Odd philosophers acquire right fork first, then left fork
       // usleep(100);
        //random_delay(50, 100);
        second_fork = philo->left_fork;
        first_fork = philo->right_fork;
    }
   

    acquire_forks(philo, first_fork, second_fork);

    update_last_meal_time(philo);
    print_status(philo, "is eating");
    //ft_usleep(philo->data->time_to_eat, philo);

    release_forks(philo, first_fork, second_fork);
} */

/* void action_eat(t_philosopher *philo)
{
    pthread_mutex_t *left_fork, *right_fork;
    int left_fork_index = philo->id - 1;
    int right_fork_index = philo->id % philo->data->number_of_philosophers;

    if (left_fork_index < right_fork_index) {
        left_fork = &philo->data->forks[left_fork_index];
        right_fork = &philo->data->forks[right_fork_index];
    } else {
        left_fork = &philo->data->forks[right_fork_index];
        right_fork = &philo->data->forks[left_fork_index];
    }

    acquire_forks(philo, left_fork, right_fork);

    update_last_meal_time(philo);
    print_status(philo, "is eating");
    //ft_usleep(philo->data->time_to_eat, philo);

    release_forks(philo, left_fork, right_fork);
} */


void action_sleep(t_philosopher *philo)
{
    print_status(philo, "is sleeping");
   // ft_usleep(philo->data->time_to_sleep , philo);
    random_delay(100, 200);  // Add a random delay to prevent immediate fork contention
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
