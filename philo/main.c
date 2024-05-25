/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 10:05:31 by jorgonca          #+#    #+#             */
/*   Updated: 2024/05/25 12:58:38 by jorgonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */




#include "philo.h"






/**
 * Handles the case where a philosopher dies.
 *
 * This function is called when a philosopher has taken a fork and is about to die.
 * It locks the left fork, prints the philosopher's status, waits for the time to die,
 * prints the philosopher's death, and then unlocks the left fork before exiting.
 *
 * @param philo The philosopher who is about to die.
 */


static t_philosopher *allocate_philosophers(int number_of_philosophers)
{
    t_philosopher *philosophers = (t_philosopher *)ft_calloc(number_of_philosophers, sizeof(t_philosopher));
    if (!philosophers)
    {
        perror("Error allocating philosophers");
        exit(EXIT_FAILURE);
    }
   // printf("Allocated %d philosophers\n", number_of_philosophers);
    return philosophers;
}

static void initialize_print_lock(t_data *data)
{
    if (pthread_mutex_init(&data->print_lock, NULL) != 0)
    {
        perror("Error initializing print_lock mutex");
       // pthread_mutex_destroy(&data->print_lock);
        exit(EXIT_FAILURE);
    }
   // printf("Initialized print_lock mutex\n");
}


static void initialize_mutexes(t_philosopher *philosophers, t_data *data)
{
    int i = 0;
    (void)philosophers;
    data->fork = (pthread_mutex_t *)ft_calloc(data->number_of_philosophers, sizeof(pthread_mutex_t));
    while (i < data->number_of_philosophers)
    {
        if (pthread_mutex_init(&data->fork[i], NULL) != 0)
        {
            perror("Error initializing fork mutex");
            for (int j = 0; j <= i; j++)
            {
                pthread_mutex_destroy(&data->fork[j]);
            }
            free(data->fork);
            exit(EXIT_FAILURE);
        }
        i++;
    }
    if (pthread_mutex_init(&data->death, NULL) != 0)
    {
        perror("Error initializing death mutex");
        for (int j = 0; j < data->number_of_philosophers; j++)
        {
            pthread_mutex_destroy(&data->fork[j]);
        }
        free(data->fork);
        exit(EXIT_FAILURE);
    }
    if (pthread_mutex_init(&data->dined, NULL) != 0)
    {
        perror("Error initializing print_lock mutex");
        for (int j = 0; j < data->number_of_philosophers; j++)
        {
            pthread_mutex_destroy(&data->fork[j]);
        }
        pthread_mutex_destroy(&data->death);
        free(data->fork);
        exit(EXIT_FAILURE);
    }
    if(pthread_mutex_init(&data->meals_eaten_mutex, NULL) != 0)
    {
        perror("Error initializing meals_eaten_mutex");
        for (int j = 0; j < data->number_of_philosophers; j++)
        {
            pthread_mutex_destroy(&data->fork[j]);
        }
        pthread_mutex_destroy(&data->death);
        pthread_mutex_destroy(&data->dined);
        free(data->fork);
        exit(EXIT_FAILURE);
    }
    if (pthread_mutex_init(&data->completed_threads_mutex, NULL) != 0)
    {
        perror("Error initializing completed_threads_mutex");
        for (int j = 0; j < data->number_of_philosophers; j++)
        {
            pthread_mutex_destroy(&data->fork[j]);
        }
        pthread_mutex_destroy(&data->death);
        pthread_mutex_destroy(&data->dined);
        pthread_mutex_destroy(&data->meals_eaten_mutex);
        free(data->fork);
        exit(EXIT_FAILURE);
    }
    //added last_meal_timestamps_mutex
    if (pthread_mutex_init(&data->last_meal_timestamps_mutex, NULL) != 0)
    {
        perror("Error initializing last_meal_timestamps_mutex");
        for (int j = 0; j < data->number_of_philosophers; j++)
        {
            pthread_mutex_destroy(&data->fork[j]);
        }
        pthread_mutex_destroy(&data->death);
        pthread_mutex_destroy(&data->dined);
        pthread_mutex_destroy(&data->meals_eaten_mutex);
        pthread_mutex_destroy(&data->completed_threads_mutex);
        free(data->fork);
        exit(EXIT_FAILURE);
    }
    data->completed_threads_count = 0;
   // printf("Initialized mutexes\n");
}

/* static void initialize_multiple_philosophers(t_philosopher *philosophers, t_data *data)
{
    int i;
    long start_time = get_current_time();
    //print_data(data);
    
    i = 0;
    while (i < data->number_of_philosophers)
    {
        data->start_time = start_time;

        philosophers[i].id = i + 1;
        philosophers[i].last_meal_time = get_current_time();
        philosophers[i].meals_eaten = 0;
        philosophers[i].time_to_die = data->time_to_die;
        philosophers[i].data = data;
        philosophers[i].data->death_note = 0;
        philosophers[i].data->dined_enough = 0;
        philosophers[i].data->completed_threads_count = 0;
        philosophers[i].data->exiting = 0;
        if (i == 0) { //check for later 
            philosophers[i].right_fork = &data->fork[data->number_of_philosophers - 1];
            philosophers[i].left_fork = &data->fork[i];
        } else if (i % 2 == 0) {
            philosophers[i].right_fork = &data->fork[philosophers[i].id - 1];
            philosophers[i].left_fork = &data->fork[philosophers[i].id % data->number_of_philosophers];
        } else {
            philosophers[i].right_fork = &data->fork[philosophers[i].id % data->number_of_philosophers];
            philosophers[i].left_fork = &data->fork[philosophers[i].id - 1];
        }
        //action for physical int of forks
        if (i == 0) { //check for later 
            philosophers[i].r_fork = &data->forks[data->number_of_philosophers - 1];
            philosophers[i].l_fork = &data->forks[i];
        } else if (i % 2 == 0) {
            philosophers[i].r_fork = &data->forks[philosophers[i].id - 1];
            philosophers[i].l_fork = &data->forks[philosophers[i].id % data->number_of_philosophers];
        } else {
            philosophers[i].r_fork = &data->forks[philosophers[i].id % data->number_of_philosophers];
            philosophers[i].l_fork = &data->forks[philosophers[i].id - 1];
        }
    //    print_philosopher(&philosophers[i]);
    //    print_data(data);
        i++;
    }
} */

// version with last_meal_timestamps
static void initialize_multiple_philosophers(t_philosopher *philosophers, t_data *data)
{
    int i;
    long start_time = get_current_time();
    //print_data(data);
    
    i = 0;
    while (i < data->number_of_philosophers)
    {
        data->start_time = start_time;

        philosophers[i].id = i + 1;
        philosophers[i].last_meal_time = get_current_time();
        philosophers[i].meals_eaten = 0;
        philosophers[i].time_to_die = data->time_to_die;
        philosophers[i].data = data;
        philosophers[i].data->death_note = 0;
        philosophers[i].data->dined_enough = 0;
        philosophers[i].data->completed_threads_count = 0;
        philosophers[i].data->exiting = 0;

        pthread_mutex_lock(&data->last_meal_timestamps_mutex);
        data->last_meal_timestamps[i] = start_time;
        pthread_mutex_unlock(&data->last_meal_timestamps_mutex);
        
        if (i == 0) { //check for later 
            philosophers[i].right_fork = &data->fork[data->number_of_philosophers - 1];
            philosophers[i].left_fork = &data->fork[i];
        } else if (i % 2 == 0) {
            philosophers[i].right_fork = &data->fork[philosophers[i].id - 1];
            philosophers[i].left_fork = &data->fork[philosophers[i].id % data->number_of_philosophers];
        } else {
            philosophers[i].right_fork = &data->fork[philosophers[i].id % data->number_of_philosophers];
            philosophers[i].left_fork = &data->fork[philosophers[i].id - 1];
        }
        //action for physical int of forks
        if (i == 0) { //check for later 
            philosophers[i].r_fork = &data->forks[data->number_of_philosophers - 1];
            philosophers[i].l_fork = &data->forks[i];
        } else if (i % 2 == 0) {
            philosophers[i].r_fork = &data->forks[philosophers[i].id - 1];
            philosophers[i].l_fork = &data->forks[philosophers[i].id % data->number_of_philosophers];
        } else {
            philosophers[i].r_fork = &data->forks[philosophers[i].id % data->number_of_philosophers];
            philosophers[i].l_fork = &data->forks[philosophers[i].id - 1];
        }
         print_philosopher(&philosophers[i]);
        print_data(data);
        i++;
    }
}

int *init_array_forks(t_data *data)
{
    int number_of_philosophers = data->number_of_philosophers; 
        int* forks = (int*) malloc(number_of_philosophers * sizeof(int));
        if (!forks)
        {
            perror("Error allocating forks");
            exit(EXIT_FAILURE);
        }

    for (int i = 0; i < number_of_philosophers; i++)
    {
        forks[i] = 0;
    }

    return forks;
}

void initialize_philosophers(t_data *data)
{
    //print_data(data);
    if (data->number_of_philosophers == 1)
    {
        initialize_single_philosopher_case(data);
        return;
    }
    t_philosopher *philosophers = allocate_philosophers(data->number_of_philosophers);
    data->philosophers = philosophers;
    initialize_print_lock(data);
    if (data->number_of_philosophers > 1)
    {
       initialize_mutexes(philosophers, data);
       initialize_multiple_philosophers(philosophers, data);
       //pthread_create(&(data->monitor), NULL, monitor_routine, data);
    }
}

/* void create_threads(t_data *data)
{
    int i = 0;
    while (i < data->number_of_philosophers)
    {
        if (pthread_create(&(data->philosophers[i].thread), NULL, philosopher_routine, &(data->philosophers[i])) != 0)
        {
            perror("Error creating philosopher thread");
            for (int j = 0; j <= i; j++)
            {
                pthread_mutex_destroy(&(data)->fork[j]);
            }
            //free(philosophers); // TODO change to free array
            pthread_mutex_destroy(&data->print_lock);
            exit(EXIT_FAILURE);
        }
        i++;
    }
} */

void create_threads(t_data *data)
{
    int i = 0;
    while (i < data->number_of_philosophers)
    {
        if (pthread_create(&(data->philosophers[i].thread), NULL, philosopher_routine, &(data->philosophers[i])) != 0)
        {
            perror("Error creating philosopher thread");
            for (int j = 0; j <= i; j++)
            {
                pthread_mutex_destroy(&(data)->fork[j]);
            }
            pthread_mutex_destroy(&data->print_lock);
            exit(EXIT_FAILURE);
        }
        usleep(100);  // Slight delay to avoid timing issues
        i++;
    }
    
    // Create the monitor thread
    if (pthread_create(&(data->monitor), NULL, monitor_routine, data) != 0)
    {
        perror("Error creating monitor thread");
        exit(EXIT_FAILURE);
    }
}

/**
 * Initializes the philosophers in the dining philosophers problem.
 *
 * This function allocates memory for the philosophers, sets their initial state,
 * and links them together in a circular list. If there is only one philosopher,
 * a special case is handled.
 *
 * @param data A pointer to the t_data struct containing the number of philosophers.
 */

int init_data(t_data *data, int argc, char **argv)
{
    data->number_of_philosophers = atoi(argv[1]);
    data->time_to_die = atol(argv[2]);
    data->time_to_eat = atol(argv[3]);
    data->time_to_sleep = atol(argv[4]);
    data->forks = init_array_forks(data);
    if (argc == 6)
    {
        data->times_must_eat = atoi(argv[5]);
        if (data->times_must_eat < 0)
        {
            write(STDERR_FILENO, "Error: times_must_eat must be a positive integer\n", 49);
            return (1);
        }
    }
    return (0);
}


int main(int argc, char **argv)
{
    t_data *data;
    
    data = ft_calloc(1, sizeof(t_data));
    if (argc != 5 && argc != 6)
    {
        write(STDERR_FILENO, "Usage:./philosophers number_of_philosophers time_to_die time_to_eat time_to_sleep [times_must_eat]\n", 75);
        return 1;
    }
    if (init_data(data, argc, argv) != 0)
    {
        write(STDERR_FILENO, "Error: invalid arguments\n", 25);
        return 1;
    }  
   
    initialize_philosophers(data);
    create_threads(data);
    if (data->number_of_philosophers > 1)
        start_simulation(data);
    clean_exit(data);
    return 0;
}
