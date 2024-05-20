/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorgonca <jorgonca@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 10:05:31 by jorgonca          #+#    #+#             */
/*   Updated: 2024/05/20 10:42:35 by jorgonca         ###   ########.fr       */
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
    printf("Allocated %d philosophers\n", number_of_philosophers);
    return philosophers;
}

static void initialize_print_lock(t_data *data)
{
    if (pthread_mutex_init(&data->print_lock, NULL) != 0)
    {
        perror("Error initializing print_lock mutex");
        exit(EXIT_FAILURE);
    }
    printf("Initialized print_lock mutex\n");
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
            /* free(philosophers);
            pthread_mutex_destroy(&data->print_lock); */
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
        /* free(philosophers);
        pthread_mutex_destroy(&data->print_lock); */
        exit(EXIT_FAILURE);
    }
    if (pthread_mutex_init(&data->print_lock, NULL) != 0)
    {
        perror("Error initializing print_lock mutex");
        for (int j = 0; j < data->number_of_philosophers; j++)
        {
            pthread_mutex_destroy(&data->fork[j]);
        }
        exit(EXIT_FAILURE);
    }
    
   // if (pthread_mutex_init(&data->, NULL) != 0)
    printf("Initialized mutexes\n");
}

static void initialize_multiple_philosophers(t_philosopher *philosophers, t_data *data)
{
    //write(1, "here\n", 5);
    int i;
    long start_time = get_current_time();
    
    i = 0;
    while (i < data->number_of_philosophers)
    {
        data->start_time = start_time;
        /* philosophers[i].left_fork = &data->fork[i];
        philosophers[i].right_fork = &data->fork[(i + 1) % data->number_of_philosophers]; */
    /* if (i == 0) {
        philosophers[i].right_fork = &data->fork[data->number_of_philosophers - 1];
    } else {
        philosophers[i].right_fork = &data->fork[i];
    }

    if (i == 0 || i % 2 == 0) {
        philosophers[i].left_fork = &data->fork[i];
    } else {
        philosophers[i].left_fork = &data->fork[i - 1];
    } */
    if (i == 0) {
        philosophers[i].right_fork = &data->fork[data->number_of_philosophers - 1];
        philosophers[i].left_fork = &data->fork[i];
    } else if (i % 2 == 0) {
        philosophers[i].left_fork = &data->fork[i];
        philosophers[i].right_fork = &data->fork[i - 1];
    } else {
        philosophers[i].right_fork = &data->fork[i];
        philosophers[i].left_fork = &data->fork[i - 1];
    }
        philosophers[i].id = i + 1;
        philosophers->last_meal_time = get_current_time();
        philosophers->meals_eaten = 0;
        philosophers->time_to_die = data->time_to_die;

        printf("Initializing philosopher %d\n", i + 1);
         //create_philosopher_thread(philosopher_routine, data, i);
         if (pthread_create(&philosophers[i].thread, NULL, &philosopher_routine, &philosophers[i]) != 0)
        {
            perror("Error creating philosopher thread");
            for (int j = 0; j <= i; j++)
            {
                pthread_mutex_destroy(&(data)->fork[j]);
            }
            free(philosophers);
            pthread_mutex_destroy(&data->print_lock);
            exit(EXIT_FAILURE);
        }
        philosophers[i].data = data;
        i++;
    }
}

void initialize_philosophers(t_data *data)
{

    if (data->number_of_philosophers == 1)
    {
        initialize_single_philosopher_case(data);
        return;
    }
    t_philosopher *philosophers = allocate_philosophers(data->number_of_philosophers);
    
    
    initialize_print_lock(data);
    if (data->number_of_philosophers > 1)
    {
       printf("DEBUG: Initializing multiple philosophers\n");
       initialize_mutexes(philosophers, data);
       initialize_multiple_philosophers(philosophers, data);
    }
    data->philosophers = philosophers;
    write(1, "DEBUG: initialize_philosophers\n", 32);
    if (data->philosophers != NULL)
    {   
        int i = 0;
        while (i < data->number_of_philosophers)
        {
            printf("================================================================================================================\n");
            printf("DEBUG: initialize_philosophers: data->philosphers %p: \n", &data[i]);
            printf("DEBUG: initialize_philosophers: data->philosophers->id %d  \n", data->philosophers[i].id);
            printf("DEBUG: initialize_philosophers: data->philosophers->last_meal_time %ld \n", data->philosophers[i].last_meal_time);
            printf("DEBUG: initialize_philosophers: data->philosophers->meals_eaten %d \n", data->philosophers[i].meals_eaten);
            printf("================================================================================================================\n");
            i++;
        }
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
    if (argc == 6)
    {
        data->times_must_eat = atoi(argv[5]);
        if (data->times_must_eat < 0)
        {
            //printf("Error: times_must_eat must be a positive integer\n");
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
        //printf("Usage: %s number_of_philosophers time_to_die time_to_eat time_to_sleep [times_must_eat]\n", argv[0]);
        write(STDERR_FILENO, "Usage:./philosophers number_of_philosophers time_to_die time_to_eat time_to_sleep [times_must_eat]\n", 75);
        return 1;
    }
    if (init_data(data, argc, argv) != 0)
    {
        write(STDERR_FILENO, "Error: invalid arguments\n", 25);
        //printf("Error: invalid arguments\n");
        return 1;
    }  
   // printf("Number of philosophers: %d\n", data->number_of_philosophers);
    //exit(0);
    initialize_philosophers(data);
    //check if the simulation should be started if there is more than one philosopher
    if (data->number_of_philosophers > 1)
        start_simulation(data);
    //write(STDOUT_FILENO, "All done!\n", 11);
    clean_exit(data);
    return 0;
}





































/* void initialize_philosophers(t_data *data)
{
    t_philosopher *philosophers = ft_calloc(data->number_of_philosophers, sizeof(t_philosopher));
    if (!philosophers)
        exit(EXIT_FAILURE);
    
    //initialize the print_lock mutex
    if (pthread_mutex_init(&data->print_lock, NULL)!= 0)
    {
        write(STDERR_FILENO, "Error initializing print_lock mutex\n", 35);
        free(philosophers);
        exit(EXIT_FAILURE);
    }

    if (data->number_of_philosophers == 1)
    {
        philosophers[0].id = 1;
        philosophers[0].last_meal_time = get_current_time();
        philosophers[0].meals_eaten = 0;
        if (pthread_mutex_init(&philosophers[0].left_fork, NULL) != 0)
        {
            //perror("Error initializing fork mutex");
            write(STDERR_FILENO, "Error initializing fork mutex\n", 36);
            free(philosophers);
            pthread_mutex_destroy(&data->print_lock);
            exit(EXIT_FAILURE);
        }
        philosophers[0].right_fork = NULL;
        philosophers[0].data = data;
        philosophers[0].next = NULL;
        philosophers[0].prev = NULL;
        data->philosophers = philosophers;
        if (pthread_create(&philosophers[0].thread, NULL, sokrates_case, &philosophers[0]) != 0)
        {
            write(STDERR_FILENO, "Error creating philosopher thread\n", 34);
            pthread_mutex_destroy(&philosophers[0].left_fork);
            free(philosophers);
            exit(EXIT_FAILURE);
        }
        return;
    }

    int i = 0;
    while (i < data->number_of_philosophers)
    {
        philosophers[i].id = i + 1;
        philosophers[i].last_meal_time = get_current_time();
        philosophers[i].meals_eaten = 0;
        pthread_mutex_init(&philosophers[i].left_fork, NULL);

        if (i == data->number_of_philosophers - 1)
            philosophers[i].right_fork = &philosophers[0].left_fork;
        else
            philosophers[i].right_fork = &philosophers[i + 1].left_fork;

        philosophers[i].data = data;
        philosophers[i].next = &philosophers[(i + 1) % data->number_of_philosophers];
        philosophers[i].prev = &philosophers[(i - 1 + data->number_of_philosophers) % data->number_of_philosophers];
        i++;
    }
    data->philosophers = philosophers;
} */
/* void initialize_philosophers(t_data *data)
{
    t_philosopher *philosophers = (t_philosopher *)calloc(data->number_of_philosophers, sizeof(t_philosopher));
    if (!philosophers)
    {
        perror("Error allocating philosophers");
        exit(EXIT_FAILURE);
    }

    // Initialize the print_lock mutex
    if (pthread_mutex_init(&data->print_lock, NULL) != 0)
    {
        perror("Error initializing print_lock mutex");
        free(philosophers);
        exit(EXIT_FAILURE);
    }

    if (data->number_of_philosophers == 1)
    {
        philosophers[0].id = 1;
        philosophers[0].last_meal_time = get_current_time();
        philosophers[0].meals_eaten = 0;
        if (pthread_mutex_init(&philosophers[0].left_fork, NULL) != 0)
        {
            perror("Error initializing fork mutex");
            free(philosophers);
            pthread_mutex_destroy(&data->print_lock);
            exit(EXIT_FAILURE);
        }
        philosophers[0].right_fork = NULL;
        philosophers[0].data = data;
        philosophers[0].next = NULL;
        philosophers[0].prev = NULL;
        data->philosophers = philosophers;
        if (pthread_create(&philosophers[0].thread, NULL, sokrates_case, &philosophers[0]) != 0)
        {
            perror("Error creating philosopher thread");
            pthread_mutex_destroy(&philosophers[0].left_fork);
            pthread_mutex_destroy(&data->print_lock);
            free(philosophers);
            exit(EXIT_FAILURE);
        }
        return;
    }

    for (int i = 0; i < data->number_of_philosophers; i++)
    {
        philosophers[i].id = i + 1;
        philosophers[i].last_meal_time = get_current_time();
        philosophers[i].meals_eaten = 0;
        if (pthread_mutex_init(&philosophers[i].left_fork, NULL) != 0)
        {
            perror("Error initializing fork mutex");
            for (int j = 0; j <= i; j++)
            {
                pthread_mutex_destroy(&philosophers[j].left_fork);
            }
            free(philosophers);
            pthread_mutex_destroy(&data->print_lock);
            exit(EXIT_FAILURE);
        }

        if (i == data->number_of_philosophers - 1)
            philosophers[i].right_fork = &philosophers[0].left_fork;
        else
            philosophers[i].right_fork = &philosophers[i + 1].left_fork;

        philosophers[i].data = data;
        philosophers[i].next = &philosophers[(i + 1) % data->number_of_philosophers];
        philosophers[i].prev = &philosophers[(i - 1 + data->number_of_philosophers) % data->number_of_philosophers];

        if (pthread_create(&philosophers[i].thread, NULL, philosopher_routine, &philosophers[i]) != 0)
        {
            perror("Error creating philosopher thread");
            for (int j = 0; j <= i; j++)
            {
                pthread_mutex_destroy(&philosophers[j].left_fork);
            }
            free(philosophers);
            pthread_mutex_destroy(&data->print_lock);
            exit(EXIT_FAILURE);
        }
    }

    data->philosophers = philosophers;
} */