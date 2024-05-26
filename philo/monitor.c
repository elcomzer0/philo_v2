/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 10:05:43 by jorgonca          #+#    #+#             */
/*   Updated: 2024/05/26 13:22:02 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */




#include "philo.h"



int dining_checker(t_philosopher *philo)
{
    pthread_mutex_lock(&philo->data->meals_eaten_mutex);
    int result = (philo->data->times_must_eat != -1 && philo->meals_eaten >= philo->data->times_must_eat);
    pthread_mutex_unlock(&philo->data->meals_eaten_mutex);
    return result;
}



int dining_timespan(t_philosopher *philo)
{
    long current_time = get_current_time();
    return (current_time - philo->last_meal_time) >= philo->data->time_to_die;
}


 void print_philosopher(t_philosopher *philosopher)
{
    printf ("----------------- This is PHILO--------------\n");
    if (philosopher == NULL) {
        printf("Philosopher data is NULL\n");
        return;
    }
    printf("Philosopher ID: %d\n", philosopher->id);
    printf("Last Meal Time: %ld\n", philosopher->last_meal_time);
    printf("Meals Eaten: %d\n", philosopher->meals_eaten);
    printf("Time to Die: %ld\n", philosopher->time_to_die);
    printf("Left Fork Address: %p\n", (void *)philosopher->l_fork);
    printf("Right Fork Address: %p\n", (void *)philosopher->r_fork);
    printf("Thread ID: %lu\n", (unsigned long)philosopher->thread);
    printf("Left Fork Mutex Address: %p\n", (void *)philosopher->left_fork);
    printf("Right Fork Mutex Address: %p\n", (void *)philosopher->right_fork);
    printf("Data Pointer: %p\n", (void *)philosopher->data);
}

void print_data(const t_data *data) {

    printf ("----------------- This is DATA--------------\n");
    printf("Start Time: %ld\n", data->start_time);
    printf("Number of Philosophers: %d\n", data->number_of_philosophers);
    printf("Time to Die: %ld\n", data->time_to_die);
    printf("Time to Eat: %ld\n", data->time_to_eat);
    printf("Time to Sleep: %ld\n", data->time_to_sleep);
    printf("Times Must Eat: %d\n", data->times_must_eat);
    printf("Death Note: %d\n", data->death_note);
    printf("Dined Enough: %d\n", data->dined_enough);

    printf("Forks: ");
    if (data->forks) {
        for (int i = 0; i < data->number_of_philosophers; i++) {
            printf("%d ", data->forks[i]);
        }
    }
    printf("\n");

    // Note: Skipping printing of pthread_t and pthread_mutex_t types
    printf("Philosophers:\n");
    if (data->philosophers) {
        for (int i = 0; i < data->number_of_philosophers; i++) {
            printf("  Philosopher %d\n", data->philosophers[i].id);
        }
    }
}


/**
 * Checks if a philosopher has dined enough times.
 *
 * This function checks if a philosopher has dined the required number of times, 
 * which is determined by the `number_of_philosophers` field in the `t_philosopher` struct.
 *  It does this by iterating through all the philosophers and checking if the `dining_checker`
 *  function returns 1 for each one. If the count reaches the `number_of_philosophers`,
 *  it sets the `dined_enough` field in the `t_philosopher` struct to 1 and returns 1 
 * to indicate that the philosopher has dined enough.
 *
 * @param philo The philosopher to check.
 * @return 1 if the philosopher has dined enough, 0 otherwise.
 */
int dined_enough(t_philosopher *philo)
{
    int i;
    int count;

    i = 0;
    count = 0;
    while (1 < philo->data->number_of_philosophers && dining_checker(philo) == 1)
    {
        count++;
        if (count == philo->data->number_of_philosophers)
        {
            pthread_mutex_lock(&philo->data->dined);
            philo->data->dined_enough = 1;
            pthread_mutex_unlock(&philo->data->dined);
            return (1);
        }
        i++;
    }
    return (0);
}


/**
 * Checks if the philosopher has a "death note" set.
 *
 * @param philo The philosopher to check.
 * @return 1 if the philosopher has a "death note" set, 0 otherwise.
 */

int death_note_check(t_philosopher *philo)
{
    if (philo == NULL || philo->data == NULL)
        return (0);
    pthread_mutex_lock(&philo->data->death);
    int death_note = philo->data->death_note;
    pthread_mutex_unlock(&philo->data->death);

    return death_note;
}


/**
 * Processes the life cycle of a philosopher in the dining philosophers problem.
 *
 * This function checks if any philosophers have died due to starvation, 
 * and updates the total number of philosophers who have dined enough.
 *
 * @param data The shared data structure containing information about the philosophers.
 * @param total_dining A pointer to the total number of philosophers who have dined enough.
 */
void process_philo_life_cycle(t_data *data, int *total_dining)
{
    long long current_time = get_current_time();
    int unlocked_inside_loop = 0; // Flag to track if the mutex was unlocked inside the loop

    pthread_mutex_lock(&data->last_meal_timestamps_mutex);
    for (int i = 0; i < data->number_of_philosophers; i++)
    {
        t_philosopher *philo = &data->philosophers[i];
        long long time_since_last_meal = current_time - data->last_meal_timestamps[i];

        if (time_since_last_meal >= data->time_to_die)
        {
            if (dining_timespan(philo)) // Use dining_timespan to check if the philosopher should die
            {
                if (philo->starvation_counter >= STARVATION_THRESHOLD)
                {
                    philo->prioritize_eating = 1;
                }
                else
                {
                    pthread_mutex_lock(&data->death);
                    if (!data->death_note) // Ensure death is reported only once
                    {
                        data->death_note = 1;
                        pthread_mutex_unlock(&data->death);
                        print_status(philo, "died");
                        pthread_mutex_unlock(&data->last_meal_timestamps_mutex);
                        unlocked_inside_loop = 1;
                        break;
                    }
                    pthread_mutex_unlock(&data->death);
                }
            }
        }
        else if (time_since_last_meal >= (data->time_to_die - THRESHOLD))
        {
            if (dined_enough(philo))
            {
                (*total_dining)++;
            }
        }
    }
    if (!unlocked_inside_loop) // Only unlock if it wasn't already unlocked inside the loop
    {
        pthread_mutex_unlock(&data->last_meal_timestamps_mutex);
    }
}




/**
 * Checks the death and eaten status of the philosophers.
 *
 * This function is responsible for checking if any philosopher has died or 
 * if all philosophers have eaten the required number of times. 
 * It updates the `dined_enough` flag accordingly and sets
 *  the `death_note` flag if a philosopher has died.
 *
 * @param data The global data structure containing information about the philosophers.
 * @param total_dining A pointer to the total number of times the philosophers have dined.
 */
void check_philo_death_and_eaten_status(t_data *data, int *total_dining)
{
    pthread_mutex_lock(&data->death);
    if (data->death_note)
    {
        pthread_mutex_unlock(&data->death);
        return;
    }
    pthread_mutex_unlock(&data->death);

    // Update dined_enough if required
    pthread_mutex_lock(&data->dined);
    if (data->times_must_eat > 0 && *total_dining >= data->number_of_philosophers)
    {
        data->dined_enough = 1;
        pthread_mutex_unlock(&data->dined);
        pthread_mutex_lock(&data->death);
        data->death_note = 1;
        pthread_mutex_unlock(&data->death);
    }
    else
    {
        pthread_mutex_unlock(&data->dined);
    }
}



/**
 * The monitor_routine function is responsible for monitoring the state of 
 * the philosophers and ensuring that no philosopher starves or dies.
 * It checks the death status of the philosophers and processes their life cycle,
 *  ensuring that they are able to eat and that no deadlocks occur.
 * The function runs in a separate thread and periodically checks 
 * the state of the philosophers, sleeping briefly between iterations 
 * to reduce CPU usage.
 *
 * @param arg A pointer to a t_data struct containing the data for the philosophers.
 * @return NULL, as this function is designed to run in a separate thread.
 */
void *monitor_routine(void *arg)
{
    printf("monitor_routine started\n");
    // usleep(500);  // Initial delay to allow philosophers to start
    t_data *data = (t_data *)arg;
    t_philosopher *philo = &(data->philosophers[0]);
    // while (!data->death_note)
    //usleep(100000);
    while (!death_note_check(philo))
    {
        // printf("monitor loop iteration\n");
        int eaten = 0;
        process_philo_life_cycle(data, &eaten);
        check_philo_death_and_eaten_status(data, &eaten);
        usleep(1000); // Short delay to reduce CPU usage
    }
    printf("monitor_routine exiting\n");
    return NULL;
}