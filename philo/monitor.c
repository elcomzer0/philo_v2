/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 10:05:43 by jorgonca          #+#    #+#             */
/*   Updated: 2024/05/25 02:57:18 by jorgonca         ###   ########.fr       */
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

/* int dining_timespan(t_philosopher *philo)
{
   if ((get_current_time() - philo->last_meal_time) >= philo->data->time_to_die)
   {
       // print_status(philo, "died");
        return (1);
   }
    return (0); 
} */

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


int dined_enough(t_philosopher *philo)
{
    int i;
    int count;

    i = 0;
    count = 0;
    while(1 < philo->data->number_of_philosophers && dining_checker(philo) == 1)
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



int death_note_check(t_philosopher *philo)
{
    if (philo == NULL || philo->data == NULL)
        return (0);
    pthread_mutex_lock(&philo->data->death);
    int death_note = philo->data->death_note;
    pthread_mutex_unlock(&philo->data->death);

    return death_note;
}


/* void process_philo_life_cycle(t_data *data, int *total_dining)
{
    for (int i = 0; i < data->number_of_philosophers; i++)
    {
        pthread_mutex_lock(&data->philosophers[i].data->death);
        if (dining_timespan(&data->philosophers[i]))
        {
            pthread_mutex_unlock(&data->philosophers[i].data->death);
            pthread_mutex_lock(&data->death);
            data->death_note = 1;
            pthread_mutex_unlock(&data->death);
            print_status(&data->philosophers[i], "died");
            break;
        }
        pthread_mutex_unlock(&data->philosophers[i].data->death);
        if (dined_enough(&data->philosophers[i]))
        {
            (*total_dining)++;
        }
    }
} */
//updated version of process_philo_life_cycle
/* void process_philo_life_cycle(t_data *data, int *total_dining)
{
    long long current_time = get_current_time();

    pthread_mutex_lock(&data->last_meal_timestamps_mutex);
    for (int i = 0; i < data->number_of_philosophers; i++)
    {
        t_philosopher *philo = &data->philosophers[i];
        long long time_since_last_meal = current_time - data->last_meal_timestamps[i];

        if (time_since_last_meal >= data->time_to_die)
        {
            if (dining_timespan(philo))
            {
                pthread_mutex_lock(&data->death);
                data->death_note = 1;
                pthread_mutex_unlock(&data->death);
                print_status(philo, "died");
                pthread_mutex_unlock(&data->last_meal_timestamps_mutex);
                break;
            }
        }
        else if (time_since_last_meal >= (data->time_to_die - (THRESHOLD)))
        {
            // Check the dined_enough condition for philosophers who have not eaten for a while
            if (dined_enough(philo))
            {
                (*total_dining)++;
            }
        }
    }
    pthread_mutex_unlock(&data->last_meal_timestamps_mutex);
} */

/* void process_philo_life_cycle(t_data *data, int *total_dining)
{
    long long current_time = get_current_time();

    pthread_mutex_lock(&data->last_meal_timestamps_mutex);
    for (int i = 0; i < data->number_of_philosophers; i++)
    {
        t_philosopher *philo = &data->philosophers[i];
        long long time_since_last_meal = current_time - data->last_meal_timestamps[i];

        if (time_since_last_meal >= data->time_to_die)
        {
            if (dining_timespan(philo))
            {
                pthread_mutex_lock(&data->death);
                data->death_note = 1;
                pthread_mutex_unlock(&data->death);
                print_status(philo, "died");
                // Ensuring not to unlock the mutex twice
                if (pthread_mutex_unlock(&data->last_meal_timestamps_mutex) != 0)
                {
                    write(2, "Error: last_meal_timestamps mutex_unlock\n", 43);
                }
                break;
            }
        }
        else if (time_since_last_meal >= (data->time_to_die - (THRESHOLD)))
        {
            // Check the dined_enough condition for philosophers who have not eaten for a while
            if (dined_enough(philo))
            {
                (*total_dining)++;
            }
        }
    }
    // Only unlock if not already unlocked inside the loop
    if (pthread_mutex_unlock(&data->last_meal_timestamps_mutex) != 0)
    {
        write(2, "Error: last_meal_timestamps mutex_unlock\n", 43);
    }
} */

/* void process_philo_life_cycle(t_data *data, int *total_dining)
{
    long long current_time = get_current_time();
    int unlocked_inside_loop = 0;  // Flag to track if the mutex was unlocked inside the loop

    pthread_mutex_lock(&data->last_meal_timestamps_mutex);
    for (int i = 0; i < data->number_of_philosophers; i++)
    {
        t_philosopher *philo = &data->philosophers[i];
        long long time_since_last_meal = current_time - data->last_meal_timestamps[i];

        if (time_since_last_meal >= data->time_to_die)
        {
            if (dining_timespan(philo))
            {
                pthread_mutex_lock(&data->death);
                data->death_note = 1;
                pthread_mutex_unlock(&data->death);
                print_status(philo, "died");
                pthread_mutex_unlock(&data->last_meal_timestamps_mutex);  // Unlock here
                unlocked_inside_loop = 1;
                break;
            }
        }
        else if (time_since_last_meal >= (data->time_to_die - (THRESHOLD)))
        {
            // Check the dined_enough condition for philosophers who have not eaten for a while
            if (dined_enough(philo))
            {
                (*total_dining)++;
            }
        }
    }
    if (!unlocked_inside_loop)  // Only unlock if it wasn't already unlocked inside the loop
    {
        pthread_mutex_unlock(&data->last_meal_timestamps_mutex);
    }
} */

void process_philo_life_cycle(t_data *data, int *total_dining)
{
    long long current_time = get_current_time();
    int unlocked_inside_loop = 0;  // Flag to track if the mutex was unlocked inside the loop

    pthread_mutex_lock(&data->last_meal_timestamps_mutex);
    for (int i = 0; i < data->number_of_philosophers; i++)
    {
        t_philosopher *philo = &data->philosophers[i];
        long long time_since_last_meal = current_time - data->last_meal_timestamps[i];

        if (time_since_last_meal >= data->time_to_die)
        {
            if (dining_timespan(philo))  // Use dining_timespan to check if the philosopher should die
            {
                pthread_mutex_lock(&data->death);
                if (!data->death_note)  // Ensure death is reported only once
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
        else if (time_since_last_meal >= (data->time_to_die - THRESHOLD))
        {
            if (dined_enough(philo))
            {
                (*total_dining)++;
            }
        }
    }
    if (!unlocked_inside_loop)  // Only unlock if it wasn't already unlocked inside the loop
    {
        pthread_mutex_unlock(&data->last_meal_timestamps_mutex);
    }
}


void check_philo_death_and_eaten_status(t_data *data, int *total_dining)
{
    pthread_mutex_lock(&data->dined);
    if (*total_dining >= data->number_of_philosophers && data->times_must_eat > 0)
    {
        pthread_mutex_lock(&data->death);
        data->death_note = 1;
        pthread_mutex_unlock(&data->death);
    }
    pthread_mutex_unlock(&data->dined);
}

void *monitor_routine(void *arg)
{
    usleep(500);
    t_data *data = (t_data *)arg;
    t_philosopher *philo = &(data->philosophers[0]);
    
    //print_philosopher(philo);
    //print_data(data);
    
    while(!death_note_check(philo))
    {
        int eaten = 0;
        process_philo_life_cycle(data, &eaten);
        check_philo_death_and_eaten_status(data, &eaten);
    }
    return NULL;
}