/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 18:21:10 by jorgonca          #+#    #+#             */
/*   Updated: 2024/05/26 21:32:15 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"



/* void action_eat(t_philosopher *philo)
{
    pthread_mutex_t *first_fork, *second_fork;

    if (philo->id % 2 == 0)
    {
        // Even philosophers acquire left fork first, then right fork
        first_fork = philo->left_fork;
        second_fork = philo->right_fork;
    }
    else
    {
        // Odd philosophers acquire right fork first, then left fork
        first_fork = philo->right_fork;
        second_fork = philo->left_fork;
    }

    if (pthread_mutex_lock(first_fork) != 0)
    {
        // Handle error
        return;
    }
    print_status(philo, "has taken a fork");

    if (pthread_mutex_lock(second_fork) != 0)
    {
        // Handle error and unlock first_fork
        pthread_mutex_unlock(first_fork);
        return;
    }
    print_status(philo, "has taken a fork");
    
     // Update the last_meal timestamp
    pthread_mutex_lock(&philo->data->last_meal_timestamps_mutex);
    philo->last_meal_time = get_current_time();
    philo->data->last_meal_timestamps[philo->id - 1] = philo->last_meal_time;
    pthread_mutex_unlock(&philo->data->last_meal_timestamps_mutex);

    print_status(philo, "is eating");
    ft_usleep(philo->data->time_to_eat * 1000, philo);

    // Update the last_meal timestamp
    philo->last_meal_time = get_current_time();

    // Acquire the meals_eaten_mutex
    pthread_mutex_lock(&philo->data->meals_eaten_mutex);
    philo->meals_eaten++;
    pthread_mutex_unlock(&philo->data->meals_eaten_mutex);

    // Check if l_fork and r_fork are valid before dereferencing
    if (philo->l_fork != NULL && philo->r_fork != NULL)
    {
        *(philo->l_fork) = 0;
        *(philo->r_fork) = 0;
    }

    pthread_mutex_unlock(second_fork);
    pthread_mutex_unlock(first_fork);
} */

/* void action_eat(t_philosopher *philo)
{
    pthread_mutex_t *first_fork, *second_fork;

    // Determine the order of fork acquisition to prevent deadlocks
    if (philo->left_fork < philo->right_fork) {
        first_fork = philo->left_fork;
        second_fork = philo->right_fork;
    } else {
        first_fork = philo->right_fork;
        second_fork = philo->left_fork;
    }

    if (pthread_mutex_lock(first_fork) != 0) {
        // Handle error
        return;
    }
    print_status(philo, "has taken a fork");

    if (pthread_mutex_lock(second_fork) != 0) {
        // Handle error and unlock first_fork
        pthread_mutex_unlock(first_fork);
        return;
    }
    print_status(philo, "has taken a fork");

    // Update the last_meal timestamp
    pthread_mutex_lock(&philo->data->last_meal_timestamps_mutex);
    philo->last_meal_time = get_current_time();
    philo->data->last_meal_timestamps[philo->id - 1] = philo->last_meal_time;
    pthread_mutex_unlock(&philo->data->last_meal_timestamps_mutex);

    print_status(philo, "is eating");
    ft_usleep(philo->data->time_to_eat * 1000, philo);

    // Update the last_meal timestamp
    philo->last_meal_time = get_current_time();

    // Acquire the meals_eaten_mutex
    pthread_mutex_lock(&philo->data->meals_eaten_mutex);
    philo->meals_eaten++;
    pthread_mutex_unlock(&philo->data->meals_eaten_mutex);

    // Check if l_fork and r_fork are valid before dereferencing
    if (philo->l_fork != NULL && philo->r_fork != NULL) {
        *(philo->l_fork) = 0;
        *(philo->r_fork) = 0;
    }

    pthread_mutex_unlock(second_fork);
    pthread_mutex_unlock(first_fork);
} */

/* void action_eat(t_philosopher *philo)
{
    pthread_mutex_t *first_fork, *second_fork;

    if (philo->id % 2 == 0)
    {
        // Even philosophers acquire left fork first, then right fork
        first_fork = philo->left_fork;
        second_fork = philo->right_fork;
    }
    else
    {
        // Odd philosophers acquire right fork first, then left fork
        first_fork = philo->right_fork;
        second_fork = philo->left_fork;
    }

    if (pthread_mutex_lock(first_fork) != 0)
    {
        return;
    }
    print_status(philo, "has taken a fork");

    if (pthread_mutex_lock(second_fork) != 0)
    {
        pthread_mutex_unlock(first_fork);
        return;
    }
    print_status(philo, "has taken a fork");

    pthread_mutex_lock(&philo->data->last_meal_timestamps_mutex);
    philo->last_meal_time = get_current_time();
    philo->data->last_meal_timestamps[philo->id - 1] = philo->last_meal_time;
    pthread_mutex_unlock(&philo->data->last_meal_timestamps_mutex);

    print_status(philo, "is eating");
    ft_usleep(philo->data->time_to_eat * 1000, philo);

    pthread_mutex_unlock(second_fork);
    pthread_mutex_unlock(first_fork);
} */

/* 
void action_eat(t_philosopher *philo)
{
    pthread_mutex_t *first_fork, *second_fork;
    if (philo->id % 2 == 0)
    {
        // Even philosophers acquire left fork first, then right fork
        first_fork = philo->left_fork;
        second_fork = philo->right_fork;
    }
    else
    {
        // Odd philosophers acquire right fork first, then left fork
        first_fork = philo->right_fork;
        second_fork = philo->left_fork;
    }

        //printf("in action_eat: Philosopher %d attempting to pick up forks at time %ld\n", philo->id, get_current_time());


    if (pthread_mutex_lock(first_fork) != 0)
    {
        return;
    }
    print_status(philo, "has taken a fork");

        //printf("in action eat: Philosopher %d attempting to pick up second fork at time %ld\n", philo->id, get_current_time());


    if (pthread_mutex_lock(second_fork) != 0)
    {
        pthread_mutex_unlock(first_fork);
        return;
    }
    print_status(philo, "has taken a fork");

    // Update the fork status
    if (philo->l_fork != NULL && philo->r_fork != NULL)
    {
        *(philo->l_fork) = 1; // Mark left fork as in use
        *(philo->r_fork) = 1; // Mark right fork as in use
    }

    // Update the last_meal timestamp
    pthread_mutex_lock(&philo->data->last_meal_timestamps_mutex);
    philo->last_meal_time = get_current_time();
    philo->data->last_meal_timestamps[philo->id - 1] = philo->last_meal_time;
    pthread_mutex_unlock(&philo->data->last_meal_timestamps_mutex);

    print_status(philo, "is eating");
    ft_usleep(philo->data->time_to_eat, philo);

    // Update the fork status
    if (philo->l_fork != NULL && philo->r_fork != NULL)
    {
        *(philo->l_fork) = 0; // Mark left fork as not in use
        *(philo->r_fork) = 0; // Mark right fork as not in use
    }

    pthread_mutex_unlock(second_fork);
    pthread_mutex_unlock(first_fork);
} */

/* void acquire_forks(t_philosopher *philo, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork) {
    if (pthread_mutex_lock(first_fork) != 0) {
        return;
    }
    print_status(philo, "has taken a fork");

    if (pthread_mutex_lock(second_fork) != 0) {
        pthread_mutex_unlock(first_fork);
        philo->starvation_counter++;
        return;
    }
    print_status(philo, "has taken a fork");
} */

/* void acquire_forks(t_philosopher *philo, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork) {
    while (1) {
        pthread_mutex_lock(first_fork);
        if (pthread_mutex_trylock(second_fork) == 0) {
            print_status(philo, "has taken a fork");
            print_status(philo, "has taken a fork");
            philo->starvation_counter = 0; // Reset the starvation counter
            break;
        } else {
            pthread_mutex_unlock(first_fork);
            philo->starvation_counter++; // Increment the starvation counter
            //usleep(50); // Sleep for a short time to avoid busy waiting
        }
    }
} */

void acquire_forks(t_philosopher *philo, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork) {
    while (1) {
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
            usleep(1000); // Sleep for a short time to avoid busy waiting
        }
    }
}


/* void release_forks(pthread_mutex_t *first_fork, pthread_mutex_t *second_fork) {
    pthread_mutex_unlock(second_fork);
    pthread_mutex_unlock(first_fork);
} */
void release_forks(t_philosopher *philo, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork) {
    pthread_mutex_lock(&philo->data->fork_status_mutex);
    *(philo->l_fork) = 0;
    *(philo->r_fork) = 0;
    pthread_mutex_unlock(&philo->data->fork_status_mutex);
    pthread_mutex_unlock(second_fork);
    pthread_mutex_unlock(first_fork);
}

/* void update_fork_status(t_philosopher *philo, int status) {
    if (philo->l_fork != NULL && philo->r_fork != NULL) {
        *(philo->l_fork) = status;
        *(philo->r_fork) = status;
    }
} */

void update_last_meal_time(t_philosopher *philo) {
    pthread_mutex_lock(&philo->data->last_meal_timestamps_mutex);
    philo->last_meal_time = get_current_time();
    philo->data->last_meal_timestamps[philo->id - 1] = philo->last_meal_time;
    pthread_mutex_unlock(&philo->data->last_meal_timestamps_mutex);
}


void action_eat(t_philosopher *philo) {
    pthread_mutex_t *first_fork, *second_fork;

    if (philo->id % 2 == 0) {
        // Even philosophers acquire left fork first, then right fork
        first_fork = philo->left_fork;
        second_fork = philo->right_fork;
    } else {
        // Odd philosophers acquire right fork first, then left fork
        first_fork = philo->right_fork;
        second_fork = philo->left_fork;
    }

    acquire_forks(philo, first_fork, second_fork);

    update_last_meal_time(philo);
    print_status(philo, "is eating");
    ft_usleep(philo->data->time_to_eat, philo);

    release_forks(philo, first_fork, second_fork);
}

/* void action_eat(t_philosopher *philo) {
    pthread_mutex_t *first_fork, *second_fork;
    int *first_fork_status, *second_fork_status;

    if (philo->id % 2 == 0) {
        // Even philosophers acquire left fork first, then right fork
        first_fork = philo->left_fork;
        second_fork = philo->right_fork;
        first_fork_status = philo->l_fork;
        second_fork_status = philo->r_fork;
    } else {
        // Odd philosophers acquire right fork first, then left fork
        first_fork = philo->right_fork;
        second_fork = philo->left_fork;
        first_fork_status = philo->r_fork;
        second_fork_status = philo->l_fork;
    }

    acquire_forks(philo, first_fork, second_fork);

    // Update fork status
    pthread_mutex_lock(&philo->data->fork_status_mutex);
    *first_fork_status = 1;
    *second_fork_status = 1;
    pthread_mutex_unlock(&philo->data->fork_status_mutex);

    update_last_meal_time(philo);
    philo->starvation_counter = 0;
    print_status(philo, "is eating");
    ft_usleep(philo->data->time_to_eat, philo);

    // Update fork status
    pthread_mutex_lock(&philo->data->fork_status_mutex);
    *first_fork_status = 0;
    *second_fork_status = 0;
    pthread_mutex_unlock(&philo->data->fork_status_mutex);

    release_forks(first_fork, second_fork);
} */




/* void action_eat(t_philosopher *philo) {
    pthread_mutex_t *first_fork, *second_fork;
    if (philo->id % 2 == 0) {
        // Even philosophers acquire left fork first, then right fork
        first_fork = philo->left_fork;
        second_fork = philo->right_fork;
    } else {
        // Odd philosophers acquire right fork first, then left fork
        first_fork = philo->right_fork;
        second_fork = philo->left_fork;
    }

    acquire_forks(philo, first_fork, second_fork);

    update_fork_status(philo, 1);
    update_last_meal_time(philo);
    philo->starvation_counter = 0;
    print_status(philo, "is eating");
    ft_usleep(philo->data->time_to_eat, philo);

    update_fork_status(philo, 0);
    release_forks(first_fork, second_fork);
} */


void action_sleep(t_philosopher *philo)
{
    print_status(philo, "is sleeping");
    ft_usleep(philo->data->time_to_sleep , philo);

}


void action_think(t_philosopher *philo)
{
    print_status(philo, "is thinking");

}
