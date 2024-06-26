/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 10:05:43 by jorgonca          #+#    #+#             */
/*   Updated: 2024/05/20 21:27:38 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */




#include "philo.h"

int dining_checker(t_philosopher *philo)
{
    //TODO protect with mutexes
    if (philo->data->times_must_eat != -1 && philo->meals_eaten >= philo->data->times_must_eat)
        return (1);
    return (0);
}
int dining_timespan(t_philosopher *philo)
{
   if ((get_current_time() - philo->last_meal_time) >= philo->data->time_to_die)
   {
        print_status(philo, "died");
        return (1);
   }
    return (0); 
}

/* int starving_check(t_philosopher *philo)
{
    int i;

    i = 0;
    while(dining_timespan(philo) == 0)
    {
        i++;
        if (i == philo->data->number_of_philosophers)
        {
            pthread_mutex_lock(&philo->data->death);
            philo->data->death_note = 1;
            pthread_mutex_unlock(&philo->data->death);
            return (1);
        }
    }
    return (0);
} */
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
    //print_philosopher(philo);
    //print_data(philo->data);
    pthread_mutex_lock(&philo->data->death);
    if (philo->data->death_note == 1)
    {
        pthread_mutex_unlock(&philo->data->death);
        return (1);
    }
    pthread_mutex_unlock(&philo->data->death);
    return (0);
}


void process_philo_life_cycle(t_data *data, int *total_dining)
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